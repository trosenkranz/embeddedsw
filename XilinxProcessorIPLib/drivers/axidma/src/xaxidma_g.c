/******************************************************************************
* Copyright (C) 2010 - 2020 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xaxidma_g.c
* @addtogroup axidma_v9_11
* @{
*
* Provide a template for user to define their own hardware settings.
*
* If using XPS, then XPS will automatically generate this file.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a jz   08/16/10 First release
* 2.00a jz   08/10/10 Second release, added in xaxidma_g.c, xaxidma_sinit.c,
*                     updated tcl file, added xaxidma_porting_guide.h
* 3.00a jz   11/22/10 Support IP core parameters change
* 4.00a rkv  02/22/11 Added support for simple DMA mode
* 6.00a srt  01/24/12 Added support for Multi-Channel DMA mode
* 7.02a srt  01/23/13 Replaced *_TDATA_WIDTH parameters to *_DATA_WIDTH
*		      (CR 691867)
* 9.7   rsp  04/25/18 In XAxiDma_Config add SG length width.
* 9.8   rsp  07/18/18 Sync XAxiDma_Config initializer fields
*
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xaxidma.h"

/************************** Constant Definitions *****************************/
#define XPAR_AXIDMA_0_INCLUDE_SG	0

XAxiDma_Config XAxiDma_ConfigTable[] =
{
	{
		0x00004712,
		0x00000000,  // Set From Filedescriptor MM address
		0, // XPAR_AXIDMA_0_SG_INCLUDE_STSCNTRL_STRM
		1, // XPAR_AXIDMA_0_INCLUDE_MM2S
		0, // XPAR_AXIDMA_0_INCLUDE_MM2S_DRE
		32, // XPAR_AXIDMA_0_M_AXI_MM2S_DATA_WIDTH
		1, // XPAR_AXIDMA_0_INCLUDE_S2MM
		0, // XPAR_AXIDMA_0_INCLUDE_S2MM_DRE
		32, // XPAR_AXIDMA_0_M_AXI_S2MM_DATA_WIDTH
		1, // XPAR_AXIDMA_0_INCLUDE_SG
		1, // XPAR_AXIDMA_0_NUM_MM2S_CHANNELS
		1, // XPAR_AXIDMA_0_NUM_S2MM_CHANNELS
		16, // XPAR_AXI_DMA_0_MM2S_BURST_SIZE
		16, // XPAR_AXI_DMA_0_MM2S_BURST_SIZE
		0,  // XPAR_AXI_DMA_0_MICRO_DMA
		32, // XPAR_AXI_DMA_0_ADDR_WIDTH // Mglw. auch nur 10, weil *.hwh Datei für AXI_LITE_ADDR_WIDTH nur 10 angibt - allerdings unwahrscheinlich, da es für die Addressbreite des zielspeichers verwendet wird.
		14 // XPAR_AXIDMA_0_SG_LENGTH_WIDTH
	}
};
/** @} */
