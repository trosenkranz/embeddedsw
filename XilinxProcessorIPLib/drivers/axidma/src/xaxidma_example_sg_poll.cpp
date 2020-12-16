/******************************************************************************
* Copyright (C) 2010 - 2020 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
 *
 * @file xaxidma_example_sg_poll.c
 *
 * This file demonstrates how to use the xaxidma driver on the Xilinx AXI
 * DMA core (AXIDMA) to transfer packets in polling mode when the AXIDMA
 * core is configured in Scatter Gather Mode.
 *
 * This code assumes a loopback hardware widget is connected to the AXI DMA
 * core for data packet loopback.
 *
 * To see the debug print, you need a Uart16550 or uartlite in your system,
 * and please set "-DDEBUG" in your compiler options. You need to rebuild your
 * software executable.
 *
 * Make sure that MEMORY_BASE is defined properly as per the HW system. The
 * h/w system built in Area mode has a maximum DDR memory limit of 64MB. In
 * throughput mode, it is 512MB.  These limits are need to ensured for
 * proper operation of this code.
 *
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.00a jz   05/17/10 First release
 * 2.00a jz   08/10/10 Second release, added in xaxidma_g.c, xaxidma_sinit.c,
 *                     updated tcl file, added xaxidma_porting_guide.h, removed
 *                     workaround for endianness
 * 4.00a rkv  02/22/11 Name of the file has been changed for naming consistency
 *       	       	   Added interrupt support for ARM.
 * 5.00a srt  03/05/12 Added Flushing and Invalidation of Caches to fix CRs
 *		       		   648103, 648701.
 *		       		   Added V7 DDR Base Address to fix CR 649405.
 * 6.00a srt  03/27/12 Changed API calls to support MCDMA driver.
 * 7.00a srt  06/18/12 API calls are reverted back for backward compatibility.
 * 7.01a srt  11/02/12 Buffer sizes (Tx and Rx) are modified to meet maximum
 *		       DDR memory limit of the h/w system built with Area mode
 * 7.02a srt  03/01/13 Updated DDR base address for IPI designs (CR 703656).
 * 9.1   adk  01/07/16 Updated DDR base address for Ultrascale (CR 799532) and
 *		       removed the defines for S6/V6.
 * 9.2   vak  15/04/16 Fixed compilation warnings in th example
 * 9.3   ms   01/23/17 Modified xil_printf statement in main function to
 *                     ensure that "Successfully ran" and "Failed" strings are
 *                     available in all examples. This is a fix for CR-965028.
 * 9.9   rsp  01/21/19 Fix use of #elif check in deriving DDR_BASE_ADDR.
 * 9.10  rsp  09/17/19 Fix cache maintenance ops for source and dest buffer.
 * </pre>
 *
 * ***************************************************************************
 */
/***************************** Include Files *********************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <functional>
#include <random>
#include <cstring>

#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
#include "xaxidma_example_sg_poll.hpp"

#ifdef __aarch64__
#include "xil_mmu.h"
#endif

#if defined(XPAR_UARTNS550_0_BASEADDR)
#include "xuartns550_l.h"       /* to use uartns550 */
#endif

//#if (!defined(DEBUG))
//extern void xil_printf(const char *format, ...);
//#endif

/******************** Constant Definitions **********************************/

/*
 * Device hardware build related constants.
 */

#define DMA_DEV_ID		0x00004712

#ifdef XPAR_AXI_7SDDR_0_S_AXI_BASEADDR
#define DDR_BASE_ADDR		XPAR_AXI_7SDDR_0_S_AXI_BASEADDR
#elif defined (XPAR_MIG7SERIES_0_BASEADDR)
#define DDR_BASE_ADDR	XPAR_MIG7SERIES_0_BASEADDR
#elif defined (XPAR_MIG_0_BASEADDR)
#define DDR_BASE_ADDR	XPAR_MIG_0_BASEADDR
#elif defined (XPAR_PSU_DDR_0_S_AXI_BASEADDR)
#define DDR_BASE_ADDR	XPAR_PSU_DDR_0_S_AXI_BASEADDR
#endif

#ifndef DDR_BASE_ADDR
#warning CHECK FOR THE VALID DDR ADDRESS IN XPARAMETERS.H, \
			DEFAULT SET TO 0x01000000
#define MEM_BASE_ADDR		0x01000000
#else
#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x1000000)
#endif

#define TX_BD_SPACE_BASE	(MEM_BASE_ADDR)
#define TX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00000FFF)
#define RX_BD_SPACE_BASE	(MEM_BASE_ADDR + 0x00001000)
#define RX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00001FFF)
#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)
#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)


#define MAX_PKT_LEN		4096*2*2-4
#define MARK_UNCACHEABLE        0x701

#define TEST_START_VALUE	0xC

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/
#if defined(XPAR_UARTNS550_0_BASEADDR)
static void Uart550_Setup(void);
#endif

static int RxSetup(XAxiDma * AxiDmaInstPtr);
static int TxSetup(XAxiDma * AxiDmaInstPtr);
static int SendPacket(XAxiDma * AxiDmaInstPtr);
static int CheckData(void);
static int CheckDmaResult(XAxiDma * AxiDmaInstPtr);

/************************** Variable Definitions *****************************/
/*
 * Device instance definitions
 */
XAxiDma AxiDma;

/*
 * Buffer for transmit packet. Must be 32-bit aligned to be used by DMA.
 */
u32 *Packet = (u32 *) TX_BUFFER_BASE;

int dmaFileDescriptor;

void* mapDMAregisterSpace() {
	const char *dev_name = "/dev/uio0";
	void* ptr;
	size_t map_size = 8 * 1024 * 1024;


    printf("Open device <%s>\n", dev_name);
	dmaFileDescriptor = open(dev_name, O_RDWR | O_SYNC);
	if (dmaFileDescriptor == -1) {
		printf("Open failed.\r\n");
		exit(1);
	}

	//printf("Map device memory\n");
	ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, dmaFileDescriptor, 0);
    if (ptr == MAP_FAILED) {
		printf("Map failed. Fehler: %s\r\n", strerror(errno));
		close(dmaFileDescriptor);
		exit(1);
    }

	//printf("Map successful.\r\n");
	//printf("DMA register space virtual address: %lu\r\n", (UINTPTR)ptr);
	//printf("DMA register space virtual address: %lx\r\n", (UINTPTR)ptr);
	return ptr;
}

void releaseDMAregisterSpace(void* ptr) {
	if (munmap(ptr, 8 * 1024 * 1024) != 0) 
		printf("Unmap failed?!?");

	close(dmaFileDescriptor);
}

int BDfileDescriptor;

void* mapBDmemorySpace()
{
	const char *dev_name = "/dev/udmabuf2";
	void* ptr;
	size_t map_size = 8 * 1024;


    printf("Open device <%s>\n", dev_name);
	BDfileDescriptor = open(dev_name, O_RDWR | O_SYNC);
	if (BDfileDescriptor == -1) {
		printf("Open failed.\r\n");
		exit(1);
	}

	//printf("Map device memory\n");
	ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, BDfileDescriptor, 0);
    if (ptr == MAP_FAILED) {
		printf("Map failed. Fehler: %s\r\n", strerror(errno));
		close(BDfileDescriptor);
		exit(1);
    }

	//printf("Map successful.\r\n");
	//printf("BD space virtual address: %lu\r\n", (UINTPTR)ptr);
	//printf("BD space virtual address: %lx\r\n", (UINTPTR)ptr);
	return ptr;
}

void releaseBDmemorySpace(void* ptr)
{
	if (munmap(ptr, 8 * 1024) != 0) 
		printf("Unmap failed?!?");

	close(BDfileDescriptor);
}

int srcFileDescriptor;

void* mapSrcMemorySpace()
{
	const char *dev_name = "/dev/udmabuf0";
	void* ptr;
	size_t map_size = 1024 * 1024;


    printf("Open device <%s>\n", dev_name);
	srcFileDescriptor = open(dev_name, O_RDWR | O_SYNC);
	if (srcFileDescriptor == -1) {
		printf("Open failed.\r\n");
		exit(1);
	}

	//printf("Map device memory\n");
	ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, srcFileDescriptor, 0);
    if (ptr == MAP_FAILED) {
		printf("Map failed. Fehler: %s\r\n", strerror(errno));
		close(srcFileDescriptor);
		exit(1);
    }

	//printf("Map successful.\r\n");
	//printf("SRC space virtual address: %lu\r\n", (UINTPTR)ptr);
	//printf("SRC space virtual address: %lx\r\n", (UINTPTR)ptr);
	return ptr;
}

void releaseSrcMemorySpace(void* ptr)
{
	if (munmap(ptr, 1024 * 1024) != 0) 
		printf("Unmap failed?!?");

	close(srcFileDescriptor);
}

int destFileDescriptor;

void* mapDestMemorySpace()
{
	const char *dev_name = "/dev/udmabuf1";
	void* ptr;
	size_t map_size = 1024 * 1024;


    printf("Open device <%s>\n", dev_name);
	destFileDescriptor = open(dev_name, O_RDWR | O_SYNC);
	if (destFileDescriptor == -1) {
		printf("Open failed.\r\n");
		exit(1);
	}

	//printf("Map device memory\n");
	ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, destFileDescriptor, 0);
    if (ptr == MAP_FAILED) {
		printf("Map failed. Fehler: %s\r\n", strerror(errno));
		close(destFileDescriptor);
		exit(1);
    }

	//printf("Map successful.\r\n");
	//printf("DEST space virtual address: %lu\r\n", (UINTPTR)ptr);
	//printf("DEST space virtual address: %lx\r\n", (UINTPTR)ptr);
	return ptr;
}

void releaseDestMemorySpace(void* ptr)
{
	if (munmap(ptr, 1024 * 1024) != 0) 
		printf("Unmap failed?!?");

	close(destFileDescriptor);
}

void fillMemoryWithRandomData(void* ptr)
{
	std::mt19937 rand(0x12345678); // Use a known initial state for rand to be able to reproduce values.

	for(uint32_t i = 0; i < (1024*1024/4); ++i) // rand returns a 32bit value, therefore we devide the buffersize by 4
		*(uint32_t*)(ptr + 4*i) = rand(); // and write only to memory locations with its two lsb bits unset...
}

void verifyMemoryContents(void* src, void* dest)
{
	int rc = std::memcmp(src, dest, 1024*1024);
    if(rc == 0)
        printf("Both memory locations contain the same content. Hurray...\r\n");
    else
		printf("Memory locations differ.\r\n");
}

UINTPTR RxBDphysicalAddress;
UINTPTR TxBDphysicalAddress;
UINTPTR RxBufferVirtualAddress;
UINTPTR TxBufferVirtualAddress;

UINTPTR getBDphysicalAddress()
{
	int fd;
	char  attr[1024];
    unsigned long  phys_addr;
    if ((fd  = open("/sys/class/u-dma-buf/udmabuf2/phys_addr", O_RDONLY)) != -1) {
        read(fd, attr, 1024);
        sscanf(attr, "%lx", &phys_addr);
        close(fd);
    }

	//printf("BD space physical address: %lu\r\n", phys_addr);
	//printf("BD space physical address: %lx\r\n", phys_addr);

	return phys_addr;
}

UINTPTR TxBufferBaseAddress;

UINTPTR getSrcPhysicalAddress()
{
	int fd;
	char  attr[1024];
    unsigned long  phys_addr;
    if ((fd  = open("/sys/class/u-dma-buf/udmabuf0/phys_addr", O_RDONLY)) != -1) {
        read(fd, attr, 1024);
        sscanf(attr, "%lx", &phys_addr);
        close(fd);
    }

	//printf("SRC space physical address: %lu\r\n", phys_addr);
	//printf("SRC space physical address: %lx\r\n", phys_addr);

	return phys_addr;
}

UINTPTR RxBufferBaseAddress;

UINTPTR getDestPhysicalAddress()
{
	int fd;
	char  attr[1024];
    unsigned long  phys_addr;
    if ((fd  = open("/sys/class/u-dma-buf/udmabuf1/phys_addr", O_RDONLY)) != -1) {
        read(fd, attr, 1024);
        sscanf(attr, "%lx", &phys_addr);
        close(fd);
    }

	//printf("DEST space physical address: %lu\r\n", phys_addr);
	//printf("DEST space physical address: %lx\r\n", phys_addr);

	return phys_addr;
}

void* TxBDmemorySpace;
void* RxBDmemorySpace;
void* dmaRegisterSpace;
void* srcMemorySpace;
void* destMemorySpace;

void printDMAregister()
{
	printf("DMA Register: \r\n");
	for(int i = 0; i <= 0x44; i +=4) {
		volatile uint32_t statusReg = *(uint32_t*)(dmaRegisterSpace + 0x40000 + i);
		printf("%x: %lx\r\n",i , statusReg);
	}
}
/*****************************************************************************/
/**
*
* Main function
*
* This function is the main entry of the tests on DMA core. It sets up
* DMA engine to be ready to receive and send packets, then a packet is
* transmitted and will be verified after it is received via the DMA loopback
* widget.
*
* @param	None
*
* @return
*		- XST_SUCCESS if test passes
*		- XST_FAILURE if test fails.
*
* @note		None.
*
******************************************************************************/
int main(void)
{
	int Status;
	XAxiDma_Config *Config;

	Status = XST_SUCCESS;

#if defined(XPAR_UARTNS550_0_BASEADDR)

	Uart550_Setup();

#endif

	printf("\r\n--- Entering main() --- \r\n");

#ifdef __aarch64__
	Xil_SetTlbAttributes(TX_BD_SPACE_BASE, MARK_UNCACHEABLE);
	Xil_SetTlbAttributes(RX_BD_SPACE_BASE, MARK_UNCACHEABLE);
#endif

	Config = XAxiDma_LookupConfig(DMA_DEV_ID);
	if (!Config) {
		printf("No config found for %d\r\n", DMA_DEV_ID);

		return XST_FAILURE;
	}

	printf("%i\r\n", sizeof(UINTPTR));

	dmaRegisterSpace = mapDMAregisterSpace();
	TxBDmemorySpace = mapBDmemorySpace();
	RxBDmemorySpace = TxBDmemorySpace + 0x1000;
	srcMemorySpace = mapSrcMemorySpace();
	destMemorySpace = mapDestMemorySpace();

	TxBufferVirtualAddress = (UINTPTR)srcMemorySpace;
	RxBufferVirtualAddress = (UINTPTR)destMemorySpace;

	printf("All Memory Spaces Mapped.\r\n");
	/*
	printf("\r\n\r\nPCI-ConfigurationRegister-Space:\r\n");
	for(int i = 0; i < 0x20; i += 4) {
		volatile uint32_t value = *((uint32_t*)(dmaRegisterSpace + i));
		printf("%x: %x\r\n", i, value);
	}

	printf("\r\n\r\nAXI-Regs:\r\n");
	for(int i = 0; i < 0x40; i += 4) {
		volatile uint32_t value = *((uint32_t*)(dmaRegisterSpace + 0x200 + i));
		printf("%x: %x\r\n", i, value);
	}
	*/

	//printDMAregister();

	// Set AXI2PCIBAR_0 to 0x3f80_0000
	//uint32_t axiOffset = 0x3f800000;
	//*((uint32_t*)(dmaRegisterSpace + 0x20C)) = axiOffset;
	*((uint32_t*)(dmaRegisterSpace + 0x20C)) = 0x0;

	//fillMemoryWithRandomData(srcMemorySpace);
	//fillMemoryWithRandomData(destMemorySpace);

	//verifyMemoryContents(srcMemorySpace, destMemorySpace);

	//RxBDphysicalAddress = getBDphysicalAddress() - axiOffset;
	TxBDphysicalAddress = getBDphysicalAddress();
	RxBDphysicalAddress = TxBDphysicalAddress + 0x1000;
	//printf("%lu\r\n", TxBDphysicalAddress);
	TxBufferBaseAddress = getSrcPhysicalAddress();
	RxBufferBaseAddress = getDestPhysicalAddress();

	Config->BaseAddr = ((UINTPTR)(dmaRegisterSpace + 0x40000));

	//printf("BaseAddress: %lx\r\n", dmaRegisterSpace);
	//printf("BaseAddress: %lx\r\n", Config->BaseAddr);

	printf("Init DMA Engine\r\n");

	/* Initialize DMA engine */
	Status = XAxiDma_CfgInitialize(&AxiDma, Config);
	if (Status != XST_SUCCESS) {
		printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	if(!XAxiDma_HasSg(&AxiDma)) {
		printf("Device configured as Simple mode \r\n");

		return XST_FAILURE;
	}

	printf("Init TX\r\n");

	Status = TxSetup(&AxiDma);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	printf("Init RX\r\n");

	Status = RxSetup(&AxiDma);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	printf("Transmit Packet\r\n");

	/* Send a packet */
	Status = SendPacket(&AxiDma);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Check DMA transfer result */
	Status = CheckDmaResult(&AxiDma);

	if (Status != XST_SUCCESS) {
		printf("AXI DMA SG Polling Example Failed\r\n");
		return XST_FAILURE;
	}

	printf("Successfully ran AXI DMA SG Polling Example\r\n");
	printf("--- Exiting main() --- \r\n");

	releaseBDmemorySpace(TxBDmemorySpace);
	releaseSrcMemorySpace(srcMemorySpace);
	releaseDestMemorySpace(destMemorySpace);
	releaseDMAregisterSpace(dmaRegisterSpace);

	printf("All Memory Spaces Unmapped. Exiting...\r\n");
	//return 0;

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

#if defined(XPAR_UARTNS550_0_BASEADDR)
/*****************************************************************************/
/*
*
* Uart16550 setup routine, need to set baudrate to 9600, and data bits to 8
*
* @param	None
*
* @return	None
*
* @note		None.
*
******************************************************************************/
static void Uart550_Setup(void)
{

	/* Set the baudrate to be predictable
	 */
	XUartNs550_SetBaud(XPAR_UARTNS550_0_BASEADDR,
			XPAR_XUARTNS550_CLOCK_HZ, 9600);

	XUartNs550_SetLineControlReg(XPAR_UARTNS550_0_BASEADDR,
			XUN_LCR_8_DATA_BITS);

}
#endif

/*****************************************************************************/
/**
*
* This function sets up RX channel of the DMA engine to be ready for packet
* reception
*
* @param	AxiDmaInstPtr is the pointer to the instance of the DMA engine.
*
* @return	XST_SUCCESS if the setup is successful, XST_FAILURE otherwise.
*
* @note		None.
*
******************************************************************************/
static int RxSetup(XAxiDma * AxiDmaInstPtr)
{
	XAxiDma_BdRing *RxRingPtr;
	int Delay = 0;
	int Coalesce = 1;
	int Status;
	XAxiDma_Bd BdTemplate;
	XAxiDma_Bd *BdPtr;
	XAxiDma_Bd *BdCurPtr;
	u32 BdCount;
	u32 FreeBdCount;
	UINTPTR RxBufferPtr;
	int Index;

	RxRingPtr = XAxiDma_GetRxRing(&AxiDma);

	/* Disable all RX interrupts before RxBD space setup */

	XAxiDma_BdRingIntDisable(RxRingPtr, XAXIDMA_IRQ_ALL_MASK);

	/* Set delay and coalescing */
	XAxiDma_BdRingSetCoalesce(RxRingPtr, Coalesce, Delay);

	/* Setup Rx BD space */
	//BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT,
	//			RX_BD_SPACE_HIGH - RX_BD_SPACE_BASE + 1);
	BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT, 0x1000); // We hardcode the number of BD in RX

	Status = XAxiDma_BdRingCreate(RxRingPtr, RxBDphysicalAddress,
				(UINTPTR)RxBDmemorySpace,
				//RxBDphysicalAddress,
				XAXIDMA_BD_MINIMUM_ALIGNMENT, BdCount);

	if (Status != XST_SUCCESS) {
		printf("RX create BD ring failed %d\r\n", Status);

		return XST_FAILURE;
	}

	/*
	 * Setup an all-zero BD as the template for the Rx channel.
	 */
	XAxiDma_BdClear(&BdTemplate);

	Status = XAxiDma_BdRingClone(RxRingPtr, &BdTemplate);
	if (Status != XST_SUCCESS) {
		printf("RX clone BD failed %d\r\n", Status);

		return XST_FAILURE;
	}

	/* Attach buffers to RxBD ring so we are ready to receive packets */

	FreeBdCount = XAxiDma_BdRingGetFreeCnt(RxRingPtr);

	Status = XAxiDma_BdRingAlloc(RxRingPtr, FreeBdCount, &BdPtr);
	if (Status != XST_SUCCESS) {
		printf("RX alloc BD failed %d\r\n", Status);

		return XST_FAILURE;
	}

	BdCurPtr = BdPtr;
	RxBufferPtr = RxBufferBaseAddress;
	for (Index = 0; Index < FreeBdCount; Index++) {
		Status = XAxiDma_BdSetBufAddr(BdCurPtr, RxBufferPtr);

		if (Status != XST_SUCCESS) {
			printf("Set buffer addr %x on BD %lx failed %d\r\n",
			    (unsigned int)RxBufferPtr,
			    (UINTPTR)BdCurPtr, Status);

			return XST_FAILURE;
		}

		Status = XAxiDma_BdSetLength(BdCurPtr, MAX_PKT_LEN,
				RxRingPtr->MaxTransferLen);
		if (Status != XST_SUCCESS) {
			printf("Rx set length %d on BD %lx failed %d\r\n",
			    MAX_PKT_LEN, (UINTPTR)BdCurPtr, Status);

			return XST_FAILURE;
		}

		/* Receive BDs do not need to set anything for the control
		 * The hardware will set the SOF/EOF bits per stream status
		 */
		XAxiDma_BdSetCtrl(BdCurPtr, 0);
		XAxiDma_BdSetId(BdCurPtr, RxBufferPtr);

		RxBufferPtr += MAX_PKT_LEN;
		BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(RxRingPtr, BdCurPtr);
	}

	/* Clear the receive buffer, so we can verify data
	 */
	memset((void *)RxBufferVirtualAddress, 0, MAX_PKT_LEN);

	Status = XAxiDma_BdRingToHw(RxRingPtr, FreeBdCount,
						BdPtr);
	if (Status != XST_SUCCESS) {
		printf("RX submit hw failed %d\r\n", Status);

		return XST_FAILURE;
	}

	/* Start RX DMA channel */
	Status = XAxiDma_BdRingStart(RxRingPtr);
	if (Status != XST_SUCCESS) {
		printf("RX start hw failed %d\r\n", Status);

		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function sets up the TX channel of a DMA engine to be ready for packet
* transmission
*
* @param	AxiDmaInstPtr is the instance pointer to the DMA engine.
*
* @return	XST_SUCCESS if the setup is successful, XST_FAILURE otherwise.
*
* @note		None.
*
******************************************************************************/
static int TxSetup(XAxiDma * AxiDmaInstPtr)
{
	XAxiDma_BdRing *TxRingPtr;
	XAxiDma_Bd BdTemplate;
	int Delay = 0;
	int Coalesce = 1;
	int Status;
	u32 BdCount;

	TxRingPtr = XAxiDma_GetTxRing(&AxiDma);

	/* Disable all TX interrupts before TxBD space setup */

	XAxiDma_BdRingIntDisable(TxRingPtr, XAXIDMA_IRQ_ALL_MASK);

	/* Set TX delay and coalesce */
	XAxiDma_BdRingSetCoalesce(TxRingPtr, Coalesce, Delay);

	/* Setup TxBD space  */
	//BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT,
	//			TX_BD_SPACE_HIGH - TX_BD_SPACE_BASE + 1);
	BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT, 0x1000); // We hardcode the number of BD in TX

	Status = XAxiDma_BdRingCreate(TxRingPtr, TxBDphysicalAddress,
				(UINTPTR)TxBDmemorySpace,
				//TxBDphysicalAddress,
				XAXIDMA_BD_MINIMUM_ALIGNMENT, BdCount);
	if (Status != XST_SUCCESS) {
		printf("failed create BD ring in txsetup\r\n");
		printf("Status: %i\r\n", Status);

		return XST_FAILURE;
	}

	/*
	 * We create an all-zero BD as the template.
	 */
	XAxiDma_BdClear(&BdTemplate);

	Status = XAxiDma_BdRingClone(TxRingPtr, &BdTemplate);
	if (Status != XST_SUCCESS) {
		printf("failed bdring clone in txsetup %d\r\n", Status);

		return XST_FAILURE;
	}

	/* Start the TX channel */
	Status = XAxiDma_BdRingStart(TxRingPtr);
	if (Status != XST_SUCCESS) {
		printf("failed start bdring txsetup %d\r\n", Status);

		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function transmits one packet non-blockingly through the DMA engine.
*
* @param	AxiDmaInstPtr points to the DMA engine instance
*
* @return	- XST_SUCCESS if the DMA accepts the packet successfully,
*		- XST_FAILURE otherwise.
*
* @note     None.
*
******************************************************************************/
static int SendPacket(XAxiDma * AxiDmaInstPtr)
{
	XAxiDma_BdRing *TxRingPtr;
	u8 *TxPacket;
	u8 Value;
	XAxiDma_Bd *BdPtr;
	int Status;
	int Index;

	TxRingPtr = XAxiDma_GetTxRing(AxiDmaInstPtr);

	/* Create pattern in the packet to transmit */
	TxPacket = (u8 *) srcMemorySpace;

	Value = TEST_START_VALUE;

	for(Index = 0; Index < MAX_PKT_LEN; Index ++) {
		TxPacket[Index] = Value;

		Value = (Value + 1) & 0xFF;
	}

	/* Flush the buffers before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	//Xil_DCacheFlushRange((UINTPTR)TxPacket, MAX_PKT_LEN);
	//Xil_DCacheFlushRange((UINTPTR)RX_BUFFER_BASE, MAX_PKT_LEN);

	/* Allocate a BD */
	Status = XAxiDma_BdRingAlloc(TxRingPtr, 1, &BdPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	printf("TX Buffer Base Address: %lx\r\n", (UINTPTR)TxBufferBaseAddress);
	/* Set up the BD using the information of the packet to transmit */
	Status = XAxiDma_BdSetBufAddr(BdPtr, (UINTPTR) TxBufferBaseAddress);
	if (Status != XST_SUCCESS) {
		printf("Tx set buffer addr %lx on BD %lx failed %d\r\n",
		    (UINTPTR)Packet, (UINTPTR)BdPtr, Status);

		return XST_FAILURE;
	}

	Status = XAxiDma_BdSetLength(BdPtr, MAX_PKT_LEN,
				TxRingPtr->MaxTransferLen);
	if (Status != XST_SUCCESS) {
		printf("Tx set length %d on BD %lx failed %d\r\n",
		    MAX_PKT_LEN, (UINTPTR)BdPtr, Status);

		return XST_FAILURE;
	}

#if (XPAR_AXIDMA_0_SG_INCLUDE_STSCNTRL_STRM == 1)
	Status = XAxiDma_BdSetAppWord(BdPtr,
	    XAXIDMA_LAST_APPWORD, MAX_PKT_LEN);

	/* If Set app length failed, it is not fatal
	 */
	if (Status != XST_SUCCESS) {
		printf("Set app word failed with %d\r\n", Status);
	}
#endif

	/* For single packet, both SOF and EOF are to be set
	 */
	XAxiDma_BdSetCtrl(BdPtr, XAXIDMA_BD_CTRL_TXEOF_MASK |
						XAXIDMA_BD_CTRL_TXSOF_MASK);

	XAxiDma_BdSetId(BdPtr, (UINTPTR)TxBufferBaseAddress); // As Id we use the physical address of the packet to send

	/* Give the BD to DMA to kick off the transmission. */
	Status = XAxiDma_BdRingToHw(TxRingPtr, 1, BdPtr);
	if (Status != XST_SUCCESS) {
		printf("to hw failed %d\r\n", Status);
		return XST_FAILURE;
	}



	return XST_SUCCESS;
}

/*****************************************************************************/
/*
*
* This function checks data buffer after the DMA transfer is finished.
*
* @param	None
*
* @return	- XST_SUCCESS if validation is successful
*		- XST_FAILURE if validation is failure.
*
* @note		None.
*
******************************************************************************/
static int CheckData(void)
{
	u8 *RxPacket;
	int Index = 0;
	u8 Value;


	RxPacket = (u8 *) RxBufferVirtualAddress;
	Value = TEST_START_VALUE;

	/* Invalidate the DestBuffer before receiving the data, in case the
	 * Data Cache is enabled
	 */
	//Xil_DCacheInvalidateRange((UINTPTR)RxPacket, MAX_PKT_LEN);

	for(Index = 0; Index < MAX_PKT_LEN; Index++) {
		if (RxPacket[Index] != Value) {
			printf("Data error %d: %x/%x\r\n",
			    Index, (unsigned int)RxPacket[Index],
			    (unsigned int)Value);

			return XST_FAILURE;
		}
		Value = (Value + 1) & 0xFF;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function waits until the DMA transaction is finished, checks data,
* and cleans up.
*
* @param	None
*
* @return	- XST_SUCCESS if DMA transfer is successful and data is correct,
*		- XST_FAILURE if fails.
*
* @note		None.
*
******************************************************************************/
static int CheckDmaResult(XAxiDma * AxiDmaInstPtr)
{
	XAxiDma_BdRing *TxRingPtr;
	XAxiDma_BdRing *RxRingPtr;
	XAxiDma_Bd *BdPtr;
	int ProcessedBdCount;
	int FreeBdCount;
	int Status;


	TxRingPtr = XAxiDma_GetTxRing(AxiDmaInstPtr);
	RxRingPtr = XAxiDma_GetRxRing(AxiDmaInstPtr);

	//printDMAregister();

	/* Wait until the one BD TX transaction is done */
	while ((ProcessedBdCount = XAxiDma_BdRingFromHw(TxRingPtr,
						       XAXIDMA_ALL_BDS,
						       &BdPtr)) == 0) {

	usleep(2000);
	}

	/* Free all processed TX BDs for future transmission */
	Status = XAxiDma_BdRingFree(TxRingPtr, ProcessedBdCount, BdPtr);
	if (Status != XST_SUCCESS) {
		printf("Failed to free %d tx BDs %d\r\n",
		    ProcessedBdCount, Status);
		return XST_FAILURE;
	}

	/* Wait until the data has been received by the Rx channel */
	while ((ProcessedBdCount = XAxiDma_BdRingFromHw(RxRingPtr,
						       XAXIDMA_ALL_BDS,
						       &BdPtr)) == 0) {
	}

	/* Check received data */
	if (CheckData() != XST_SUCCESS) {

		return XST_FAILURE;
	}

	/* Free all processed RX BDs for future transmission */
	Status = XAxiDma_BdRingFree(RxRingPtr, ProcessedBdCount, BdPtr);
	if (Status != XST_SUCCESS) {
		printf("Failed to free %d rx BDs %d\r\n",
		    ProcessedBdCount, Status);
		return XST_FAILURE;
	}

	/* Return processed BDs to RX channel so we are ready to receive new
	 * packets:
	 *    - Allocate all free RX BDs
	 *    - Pass the BDs to RX channel
	 */
	FreeBdCount = XAxiDma_BdRingGetFreeCnt(RxRingPtr);
	Status = XAxiDma_BdRingAlloc(RxRingPtr, FreeBdCount, &BdPtr);
	if (Status != XST_SUCCESS) {
		printf("bd alloc failed\r\n");
		return XST_FAILURE;
	}

	Status = XAxiDma_BdRingToHw(RxRingPtr, FreeBdCount, BdPtr);
	if (Status != XST_SUCCESS) {
		printf("Submit %d rx BDs failed %d\r\n", FreeBdCount, Status);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
