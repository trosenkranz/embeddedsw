/******************************************************************************
*
* Copyright (C) 2015-2018 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*****************************************************************************/
/**
 * @file pm_defs.h
 *
 * PM Definitions implementation
 * @addtogroup xpm_apis XilPM APIs
 * @{
 *****************************************************************************/

#ifndef PM_DEFS_H_
#define PM_DEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @name PM Version Number macros
 *
 * @{
 */
#define PM_VERSION_MAJOR	1
#define PM_VERSION_MINOR	1

#define PM_VERSION	((PM_VERSION_MAJOR << 16) | PM_VERSION_MINOR)
/**@}*/

/** @name Capabilities for RAM
 *
 * @{
 */
#define PM_CAP_ACCESS	0x1U
#define PM_CAP_CONTEXT	0x2U
#define PM_CAP_WAKEUP	0x4U
/**@}*/

/** @name Node default states macros
 *
 * @{
 */
#define NODE_STATE_OFF		0
#define NODE_STATE_ON		1
/**@}*/

/** @name Processor's states macros
 *
 * @{
 */
#define PROC_STATE_FORCEDOFF	0
#define PROC_STATE_ACTIVE	1
#define PROC_STATE_SLEEP	2
#define PROC_STATE_SUSPENDING	3
/**@}*/

/** @name Maximum Latency/QOS macros
 *
 * @{
 */
#define MAX_LATENCY	(~0U)
#define MAX_QOS		100U
/**@}*/

/** @name System shutdown/Restart macros
 *
 * @{
 */
#define PMF_SHUTDOWN_TYPE_SHUTDOWN	0U
#define PMF_SHUTDOWN_TYPE_RESET		1U

#define PMF_SHUTDOWN_SUBTYPE_SUBSYSTEM	0U
#define PMF_SHUTDOWN_SUBTYPE_PS_ONLY	1U
#define PMF_SHUTDOWN_SUBTYPE_SYSTEM	2U
/**@}*/

#define PM_CLOCK_DIV0_ID	0U
#define PM_CLOCK_DIV1_ID	1U

/**
 *  APIs for Miscellaneous functions, suspending of PUs, managing PM slaves and Direct control.
 */
enum XPmApiId {
	/* Miscellaneous API functions: */
	PM_GET_API_VERSION = 1, /* Do not change or move */
	PM_SET_CONFIGURATION,
	PM_GET_NODE_STATUS,
	PM_GET_OP_CHARACTERISTIC,
	PM_REGISTER_NOTIFIER,
	/*  API for suspending of PUs: */
	PM_REQUEST_SUSPEND,
	PM_SELF_SUSPEND,
	PM_FORCE_POWERDOWN,
	PM_ABORT_SUSPEND,
	PM_REQUEST_WAKEUP,
	PM_SET_WAKEUP_SOURCE,
	PM_SYSTEM_SHUTDOWN,
	/*API for managing PM slaves: */
	PM_REQUEST_NODE,
	PM_RELEASE_NODE,
	PM_SET_REQUIREMENT,
	PM_SET_MAX_LATENCY,
	/* Direct control API functions: */
	PM_RESET_ASSERT,
	PM_RESET_GET_STATUS,
	PM_MMIO_WRITE,
	PM_MMIO_READ,
	PM_INIT_FINALIZE,
	PM_FPGA_LOAD,
	PM_FPGA_GET_STATUS,
	PM_GET_CHIPID,
	/* Secure library generic API functions */
	PM_SECURE_SHA = 26U,
	PM_SECURE_RSA,
	PM_PINCTRL_REQUEST,
	PM_PINCTRL_RELEASE,
	PM_PINCTRL_GET_FUNCTION,
	PM_PINCTRL_SET_FUNCTION,
	PM_PINCTRL_CONFIG_PARAM_GET,
	PM_PINCTRL_CONFIG_PARAM_SET,
	/* PM IOCTL API */
	PM_IOCTL,
	/* API to query information from firmware */
	PM_QUERY_DATA,
	/* Clock control API functions */
	PM_CLOCK_ENABLE,
	PM_CLOCK_DISABLE,
	PM_CLOCK_GETSTATE,
	PM_CLOCK_SETDIVIDER,
	PM_CLOCK_GETDIVIDER,
	PM_CLOCK_SETRATE,
	PM_CLOCK_GETRATE,
	PM_CLOCK_SETPARENT,
	PM_CLOCK_GETPARENT,
	/* Secure image */
	PM_SECURE_IMAGE,
	PM_FPGA_READ,
	PM_SECURE_AES,
	/* PLL direct control API functions */
	PM_PLL_SET_PARAMETER,
	PM_PLL_GET_PARAMETER,
	PM_PLL_SET_MODE,
	PM_PLL_GET_MODE,
	PM_REGISTER_ACCESS,
	PM_EFUSE_ACCESS,
	PM_API_MAX
};

/** @name PM API Min and Max macros
 *
 * @{
 */
#define PM_API_MIN	PM_GET_API_VERSION
/**@}*/

/**
 *  PM API Callback Id Enum
 */
enum XPmApiCbId {
	PM_INIT_SUSPEND_CB = 30,
	PM_ACKNOWLEDGE_CB,
	PM_NOTIFY_CB,
};

/**
 * PM Node ID Enum
 */
enum XPmNodeId {
	NODE_UNKNOWN,
	NODE_APU,
	NODE_APU_0,
	NODE_APU_1,
	NODE_APU_2,
	NODE_APU_3,
	NODE_RPU,
	NODE_RPU_0,
	NODE_RPU_1,
	NODE_PLD,
	NODE_FPD,
	NODE_OCM_BANK_0,
	NODE_OCM_BANK_1,
	NODE_OCM_BANK_2,
	NODE_OCM_BANK_3,
	NODE_TCM_0_A,
	NODE_TCM_0_B,
	NODE_TCM_1_A,
	NODE_TCM_1_B,
	NODE_L2,
	NODE_GPU_PP_0,
	NODE_GPU_PP_1,
	NODE_USB_0,
	NODE_USB_1,
	NODE_TTC_0,
	NODE_TTC_1,
	NODE_TTC_2,
	NODE_TTC_3,
	NODE_SATA,
	NODE_ETH_0,
	NODE_ETH_1,
	NODE_ETH_2,
	NODE_ETH_3,
	NODE_UART_0,
	NODE_UART_1,
	NODE_SPI_0,
	NODE_SPI_1,
	NODE_I2C_0,
	NODE_I2C_1,
	NODE_SD_0,
	NODE_SD_1,
	NODE_DP,
	NODE_GDMA,
	NODE_ADMA,
	NODE_NAND,
	NODE_QSPI,
	NODE_GPIO,
	NODE_CAN_0,
	NODE_CAN_1,
	NODE_EXTERN,
	NODE_APLL,
	NODE_VPLL,
	NODE_DPLL,
	NODE_RPLL,
	NODE_IOPLL,
	NODE_DDR,
	NODE_IPI_APU,
	NODE_IPI_RPU_0,
	NODE_GPU,
	NODE_PCIE,
	NODE_PCAP,
	NODE_RTC,
	NODE_LPD,
	NODE_VCU,
	NODE_IPI_RPU_1,
	NODE_IPI_PL_0,
	NODE_IPI_PL_1,
	NODE_IPI_PL_2,
	NODE_IPI_PL_3,
	NODE_PL,
	NODE_ID_MAX
};

/**
 *  PM Acknowledge Request Types
 */
enum XPmRequestAck {
	REQUEST_ACK_NO = 1,
	REQUEST_ACK_BLOCKING,
	REQUEST_ACK_NON_BLOCKING,
	REQUEST_ACK_CB_CERROR,
};

/**
 *  PM Abort Reasons Enum
 */
enum XPmAbortReason {
	ABORT_REASON_WKUP_EVENT = 100,
	ABORT_REASON_PU_BUSY,
	ABORT_REASON_NO_PWRDN,
	ABORT_REASON_UNKNOWN,
};

/**
 *  PM Suspend Reasons Enum
 */
enum XPmSuspendReason {
	SUSPEND_REASON_PU_REQ = 201,
	SUSPEND_REASON_ALERT,
	SUSPEND_REASON_SYS_SHUTDOWN,
};

/**
 *  PM RAM States Enum
 */
enum XPmRamState {
	PM_RAM_STATE_OFF = 0,
	PM_RAM_STATE_RETENTION,
	PM_RAM_STATE_ON,
};

/**
 *  PM Operating Characteristic types Enum
 */
enum XPmOpCharType {
	PM_OPCHAR_TYPE_POWER = 1,
	PM_OPCHAR_TYPE_TEMP,
	PM_OPCHAR_TYPE_LATENCY,
};

 /* Power management specific return error statuses */
/** @defgroup pmstatmacro
 * @{
 */
/** An internal error occurred while performing the requested operation */
#define XST_PM_INTERNAL  2000L
/** Conflicting requirements have been asserted when more than one processing
 * cluster is using the same PM slave */
#define XST_PM_CONFLICT  2001L
/** The processing cluster does not have access to the requested node or
 * operation */
#define XST_PM_NO_ACCESS  2002L
/** The API function does not apply to the node passed as argument */
#define XST_PM_INVALID_NODE  2003L
/**  A processing cluster has already been assigned access to a PM slave and
 * has issued a duplicate request for that PM slave */
#define XST_PM_DOUBLE_REQ  2004L
/** The target processing cluster has aborted suspend */
#define XST_PM_ABORT_SUSPEND  2005L
/** A timeout occurred while performing the requested operation*/
#define XST_PM_TIMEOUT   2006L
/**  Slave request cannot be granted since node is non-shareable and used */
#define XST_PM_NODE_USED  2007L
/**@}*/

/**
 *  Boot Status Enum
 */
enum XPmBootStatus {
	PM_INITIAL_BOOT,	/* boot is a fresh system startup */
	PM_RESUME,			/* boot is a resume */
	PM_BOOT_ERROR,		/* error, boot cause cannot be identified */
};

/**
 *  PM Reset Action types
 */
enum XPmResetAction {
	XILPM_RESET_ACTION_RELEASE,
	XILPM_RESET_ACTION_ASSERT,
	XILPM_RESET_ACTION_PULSE,
};

/**
 *  PM Reset Line IDs
 */
enum XPmReset {
	XILPM_RESET_PCIE_CFG = 1000,
	XILPM_RESET_PCIE_BRIDGE,
	XILPM_RESET_PCIE_CTRL,
	XILPM_RESET_DP,
	XILPM_RESET_SWDT_CRF,
	XILPM_RESET_AFI_FM5,
	XILPM_RESET_AFI_FM4,
	XILPM_RESET_AFI_FM3,
	XILPM_RESET_AFI_FM2,
	XILPM_RESET_AFI_FM1,
	XILPM_RESET_AFI_FM0,
	XILPM_RESET_GDMA,
	XILPM_RESET_GPU_PP1,
	XILPM_RESET_GPU_PP0,
	XILPM_RESET_GPU,
	XILPM_RESET_GT,
	XILPM_RESET_SATA,
	XILPM_RESET_ACPU3_PWRON,
	XILPM_RESET_ACPU2_PWRON,
	XILPM_RESET_ACPU1_PWRON,
	XILPM_RESET_ACPU0_PWRON,
	XILPM_RESET_APU_L2,
	XILPM_RESET_ACPU3,
	XILPM_RESET_ACPU2,
	XILPM_RESET_ACPU1,
	XILPM_RESET_ACPU0,
	XILPM_RESET_DDR,
	XILPM_RESET_APM_FPD,
	XILPM_RESET_SOFT,
	XILPM_RESET_GEM0,
	XILPM_RESET_GEM1,
	XILPM_RESET_GEM2,
	XILPM_RESET_GEM3,
	XILPM_RESET_QSPI,
	XILPM_RESET_UART0,
	XILPM_RESET_UART1,
	XILPM_RESET_SPI0,
	XILPM_RESET_SPI1,
	XILPM_RESET_SDIO0,
	XILPM_RESET_SDIO1,
	XILPM_RESET_CAN0,
	XILPM_RESET_CAN1,
	XILPM_RESET_I2C0,
	XILPM_RESET_I2C1,
	XILPM_RESET_TTC0,
	XILPM_RESET_TTC1,
	XILPM_RESET_TTC2,
	XILPM_RESET_TTC3,
	XILPM_RESET_SWDT_CRL,
	XILPM_RESET_NAND,
	XILPM_RESET_ADMA,
	XILPM_RESET_GPIO,
	XILPM_RESET_IOU_CC,
	XILPM_RESET_TIMESTAMP,
	XILPM_RESET_RPU_R50,
	XILPM_RESET_RPU_R51,
	XILPM_RESET_RPU_AMBA,
	XILPM_RESET_OCM,
	XILPM_RESET_RPU_PGE,
	XILPM_RESET_USB0_CORERESET,
	XILPM_RESET_USB1_CORERESET,
	XILPM_RESET_USB0_HIBERRESET,
	XILPM_RESET_USB1_HIBERRESET,
	XILPM_RESET_USB0_APB,
	XILPM_RESET_USB1_APB,
	XILPM_RESET_IPI,
	XILPM_RESET_APM_LPD,
	XILPM_RESET_RTC,
	XILPM_RESET_SYSMON,
	XILPM_RESET_AFI_FM6,
	XILPM_RESET_LPD_SWDT,
	XILPM_RESET_FPD,
	XILPM_RESET_RPU_DBG1,
	XILPM_RESET_RPU_DBG0,
	XILPM_RESET_DBG_LPD,
	XILPM_RESET_DBG_FPD,
	XILPM_RESET_APLL,
	XILPM_RESET_DPLL,
	XILPM_RESET_VPLL,
	XILPM_RESET_IOPLL,
	XILPM_RESET_RPLL,
	XILPM_RESET_GPO3_PL_0,
	XILPM_RESET_GPO3_PL_1,
	XILPM_RESET_GPO3_PL_2,
	XILPM_RESET_GPO3_PL_3,
	XILPM_RESET_GPO3_PL_4,
	XILPM_RESET_GPO3_PL_5,
	XILPM_RESET_GPO3_PL_6,
	XILPM_RESET_GPO3_PL_7,
	XILPM_RESET_GPO3_PL_8,
	XILPM_RESET_GPO3_PL_9,
	XILPM_RESET_GPO3_PL_10,
	XILPM_RESET_GPO3_PL_11,
	XILPM_RESET_GPO3_PL_12,
	XILPM_RESET_GPO3_PL_13,
	XILPM_RESET_GPO3_PL_14,
	XILPM_RESET_GPO3_PL_15,
	XILPM_RESET_GPO3_PL_16,
	XILPM_RESET_GPO3_PL_17,
	XILPM_RESET_GPO3_PL_18,
	XILPM_RESET_GPO3_PL_19,
	XILPM_RESET_GPO3_PL_20,
	XILPM_RESET_GPO3_PL_21,
	XILPM_RESET_GPO3_PL_22,
	XILPM_RESET_GPO3_PL_23,
	XILPM_RESET_GPO3_PL_24,
	XILPM_RESET_GPO3_PL_25,
	XILPM_RESET_GPO3_PL_26,
	XILPM_RESET_GPO3_PL_27,
	XILPM_RESET_GPO3_PL_28,
	XILPM_RESET_GPO3_PL_29,
	XILPM_RESET_GPO3_PL_30,
	XILPM_RESET_GPO3_PL_31,
	XILPM_RESET_RPU_LS,
	XILPM_RESET_PS_ONLY,
	XILPM_RESET_PL,
	XILPM_RESET_GPIO5_EMIO_92,
	XILPM_RESET_GPIO5_EMIO_93,
	XILPM_RESET_GPIO5_EMIO_94,
	XILPM_RESET_GPIO5_EMIO_95,
};

/**
 *  PM Notify Events Enum
 */
enum XPmNotifyEvent {
	EVENT_STATE_CHANGE = 1,
	EVENT_ZERO_USERS = 2,
};

/**
 *  PM Clock IDs
 */
enum XPmClock {
	PM_CLOCK_IOPLL,
	PM_CLOCK_RPLL,
	PM_CLOCK_APLL,
	PM_CLOCK_DPLL,
	PM_CLOCK_VPLL,
	PM_CLOCK_IOPLL_TO_FPD,
	PM_CLOCK_RPLL_TO_FPD,
	PM_CLOCK_APLL_TO_LPD,
	PM_CLOCK_DPLL_TO_LPD,
	PM_CLOCK_VPLL_TO_LPD,
	PM_CLOCK_ACPU,
	PM_CLOCK_ACPU_HALF,
	PM_CLOCK_DBG_FPD,
	PM_CLOCK_DBG_LPD,
	PM_CLOCK_DBG_TRACE,
	PM_CLOCK_DBG_TSTMP,
	PM_CLOCK_DP_VIDEO_REF,
	PM_CLOCK_DP_AUDIO_REF,
	PM_CLOCK_DP_STC_REF,
	PM_CLOCK_GDMA_REF,
	PM_CLOCK_DPDMA_REF,
	PM_CLOCK_DDR_REF,
	PM_CLOCK_SATA_REF,
	PM_CLOCK_PCIE_REF,
	PM_CLOCK_GPU_REF,
	PM_CLOCK_GPU_PP0_REF,
	PM_CLOCK_GPU_PP1_REF,
	PM_CLOCK_TOPSW_MAIN,
	PM_CLOCK_TOPSW_LSBUS,
	PM_CLOCK_GTGREF0_REF,
	PM_CLOCK_LPD_SWITCH,
	PM_CLOCK_LPD_LSBUS,
	PM_CLOCK_USB0_BUS_REF,
	PM_CLOCK_USB1_BUS_REF,
	PM_CLOCK_USB3_DUAL_REF,
	PM_CLOCK_USB0,
	PM_CLOCK_USB1,
	PM_CLOCK_CPU_R5,
	PM_CLOCK_CPU_R5_CORE,
	PM_CLOCK_CSU_SPB,
	PM_CLOCK_CSU_PLL,
	PM_CLOCK_PCAP,
	PM_CLOCK_IOU_SWITCH,
	PM_CLOCK_GEM_TSU_REF,
	PM_CLOCK_GEM_TSU,
	PM_CLOCK_GEM0_TX,
	PM_CLOCK_GEM1_TX,
	PM_CLOCK_GEM2_TX,
	PM_CLOCK_GEM3_TX,
	PM_CLOCK_GEM0_RX,
	PM_CLOCK_GEM1_RX,
	PM_CLOCK_GEM2_RX,
	PM_CLOCK_GEM3_RX,
	PM_CLOCK_QSPI_REF,
	PM_CLOCK_SDIO0_REF,
	PM_CLOCK_SDIO1_REF,
	PM_CLOCK_UART0_REF,
	PM_CLOCK_UART1_REF,
	PM_CLOCK_SPI0_REF,
	PM_CLOCK_SPI1_REF,
	PM_CLOCK_NAND_REF,
	PM_CLOCK_I2C0_REF,
	PM_CLOCK_I2C1_REF,
	PM_CLOCK_CAN0_REF,
	PM_CLOCK_CAN1_REF,
	PM_CLOCK_CAN0,
	PM_CLOCK_CAN1,
	PM_CLOCK_DLL_REF,
	PM_CLOCK_ADMA_REF,
	PM_CLOCK_TIMESTAMP_REF,
	PM_CLOCK_AMS_REF,
	PM_CLOCK_PL0_REF,
	PM_CLOCK_PL1_REF,
	PM_CLOCK_PL2_REF,
	PM_CLOCK_PL3_REF,
	PM_CLOCK_WDT,
	PM_CLOCK_IOPLL_INT,
	PM_CLOCK_IOPLL_PRE_SRC,
	PM_CLOCK_IOPLL_HALF,
	PM_CLOCK_IOPLL_INT_MUX,
	PM_CLOCK_IOPLL_POST_SRC,
	PM_CLOCK_RPLL_INT,
	PM_CLOCK_RPLL_PRE_SRC,
	PM_CLOCK_RPLL_HALF,
	PM_CLOCK_RPLL_INT_MUX,
	PM_CLOCK_RPLL_POST_SRC,
	PM_CLOCK_APLL_INT,
	PM_CLOCK_APLL_PRE_SRC,
	PM_CLOCK_APLL_HALF,
	PM_CLOCK_APLL_INT_MUX,
	PM_CLOCK_APLL_POST_SRC,
	PM_CLOCK_DPLL_INT,
	PM_CLOCK_DPLL_PRE_SRC,
	PM_CLOCK_DPLL_HALF,
	PM_CLOCK_DPLL_INT_MUX,
	PM_CLOCK_DPLL_POST_SRC,
	PM_CLOCK_VPLL_INT,
	PM_CLOCK_VPLL_PRE_SRC,
	PM_CLOCK_VPLL_HALF,
	PM_CLOCK_VPLL_INT_MUX,
	PM_CLOCK_VPLL_POST_SRC,
	PM_CLOCK_CAN0_MIO,
	PM_CLOCK_CAN1_MIO,
	PM_CLOCK_ACPU_FULL,
	PM_CLOCK_GEM0_REF,
	PM_CLOCK_GEM1_REF,
	PM_CLOCK_GEM2_REF,
	PM_CLOCK_GEM3_REF,
	PM_CLOCK_GEM0_REF_UNGATED,
	PM_CLOCK_GEM1_REF_UNGATED,
	PM_CLOCK_GEM2_REF_UNGATED,
	PM_CLOCK_GEM3_REF_UNGATED,
	PM_CLOCK_EXT_PSS_REF,
	PM_CLOCK_EXT_VIDEO,
	PM_CLOCK_EXT_PSS_ALT_REF,
	PM_CLOCK_EXT_AUX_REF,
	PM_CLOCK_EXT_GT_CRX_REF,
	PM_CLOCK_EXT_SWDT0,
	PM_CLOCK_EXT_SWDT1,
	PM_CLOCK_EXT_GEM0_TX_EMIO,
	PM_CLOCK_EXT_GEM1_TX_EMIO,
	PM_CLOCK_EXT_GEM2_TX_EMIO,
	PM_CLOCK_EXT_GEM3_TX_EMIO,
	PM_CLOCK_EXT_GEM0_RX_EMIO,
	PM_CLOCK_EXT_GEM1_RX_EMIO,
	PM_CLOCK_EXT_GEM2_RX_EMIO,
	PM_CLOCK_EXT_GEM3_RX_EMIO,
	PM_CLOCK_EXT_MIO50_OR_MIO51,
	PM_CLOCK_EXT_MIO0,
	PM_CLOCK_EXT_MIO1,
	PM_CLOCK_EXT_MIO2,
	PM_CLOCK_EXT_MIO3,
	PM_CLOCK_EXT_MIO4,
	PM_CLOCK_EXT_MIO5,
	PM_CLOCK_EXT_MIO6,
	PM_CLOCK_EXT_MIO7,
	PM_CLOCK_EXT_MIO8,
	PM_CLOCK_EXT_MIO9,
	PM_CLOCK_EXT_MIO10,
	PM_CLOCK_EXT_MIO11,
	PM_CLOCK_EXT_MIO12,
	PM_CLOCK_EXT_MIO13,
	PM_CLOCK_EXT_MIO14,
	PM_CLOCK_EXT_MIO15,
	PM_CLOCK_EXT_MIO16,
	PM_CLOCK_EXT_MIO17,
	PM_CLOCK_EXT_MIO18,
	PM_CLOCK_EXT_MIO19,
	PM_CLOCK_EXT_MIO20,
	PM_CLOCK_EXT_MIO21,
	PM_CLOCK_EXT_MIO22,
	PM_CLOCK_EXT_MIO23,
	PM_CLOCK_EXT_MIO24,
	PM_CLOCK_EXT_MIO25,
	PM_CLOCK_EXT_MIO26,
	PM_CLOCK_EXT_MIO27,
	PM_CLOCK_EXT_MIO28,
	PM_CLOCK_EXT_MIO29,
	PM_CLOCK_EXT_MIO30,
	PM_CLOCK_EXT_MIO31,
	PM_CLOCK_EXT_MIO32,
	PM_CLOCK_EXT_MIO33,
	PM_CLOCK_EXT_MIO34,
	PM_CLOCK_EXT_MIO35,
	PM_CLOCK_EXT_MIO36,
	PM_CLOCK_EXT_MIO37,
	PM_CLOCK_EXT_MIO38,
	PM_CLOCK_EXT_MIO39,
	PM_CLOCK_EXT_MIO40,
	PM_CLOCK_EXT_MIO41,
	PM_CLOCK_EXT_MIO42,
	PM_CLOCK_EXT_MIO43,
	PM_CLOCK_EXT_MIO44,
	PM_CLOCK_EXT_MIO45,
	PM_CLOCK_EXT_MIO46,
	PM_CLOCK_EXT_MIO47,
	PM_CLOCK_EXT_MIO48,
	PM_CLOCK_EXT_MIO49,
	PM_CLOCK_EXT_MIO50,
	PM_CLOCK_EXT_MIO51,
	PM_CLOCK_EXT_MIO52,
	PM_CLOCK_EXT_MIO53,
	PM_CLOCK_EXT_MIO54,
	PM_CLOCK_EXT_MIO55,
	PM_CLOCK_EXT_MIO56,
	PM_CLOCK_EXT_MIO57,
	PM_CLOCK_EXT_MIO58,
	PM_CLOCK_EXT_MIO59,
	PM_CLOCK_EXT_MIO60,
	PM_CLOCK_EXT_MIO61,
	PM_CLOCK_EXT_MIO62,
	PM_CLOCK_EXT_MIO63,
	PM_CLOCK_EXT_MIO64,
	PM_CLOCK_EXT_MIO65,
	PM_CLOCK_EXT_MIO66,
	PM_CLOCK_EXT_MIO67,
	PM_CLOCK_EXT_MIO68,
	PM_CLOCK_EXT_MIO69,
	PM_CLOCK_EXT_MIO70,
	PM_CLOCK_EXT_MIO71,
	PM_CLOCK_EXT_MIO72,
	PM_CLOCK_EXT_MIO73,
	PM_CLOCK_EXT_MIO74,
	PM_CLOCK_EXT_MIO75,
	PM_CLOCK_EXT_MIO76,
	PM_CLOCK_EXT_MIO77,
};

enum XPmPllParam {
	PM_PLL_PARAM_ID_DIV2,
	PM_PLL_PARAM_ID_FBDIV,
	PM_PLL_PARAM_ID_DATA,
	PM_PLL_PARAM_ID_PRE_SRC,
	PM_PLL_PARAM_ID_POST_SRC,
	PM_PLL_PARAM_ID_LOCK_DLY,
	PM_PLL_PARAM_ID_LOCK_CNT,
	PM_PLL_PARAM_ID_LFHF,
	PM_PLL_PARAM_ID_CP,
	PM_PLL_PARAM_ID_RES,
};

enum XPmPllMode {
	PM_PLL_MODE_RESET,
	PM_PLL_MODE_INTEGER,
	PM_PLL_MODE_FRACTIONAL,
};

enum XPmPinFn {
    PINCTRL_FUNC_CAN0,
    PINCTRL_FUNC_CAN1,
    PINCTRL_FUNC_ETHERNET0,
    PINCTRL_FUNC_ETHERNET1,
    PINCTRL_FUNC_ETHERNET2,
    PINCTRL_FUNC_ETHERNET3,
    PINCTRL_FUNC_GEMTSU0,
    PINCTRL_FUNC_GPIO0,
    PINCTRL_FUNC_I2C0,
    PINCTRL_FUNC_I2C1,
    PINCTRL_FUNC_MDIO0,
    PINCTRL_FUNC_MDIO1,
    PINCTRL_FUNC_MDIO2,
    PINCTRL_FUNC_MDIO3,
    PINCTRL_FUNC_QSPI0,
    PINCTRL_FUNC_QSPI_FBCLK,
    PINCTRL_FUNC_QSPI_SS,
    PINCTRL_FUNC_SPI0,
    PINCTRL_FUNC_SPI1,
    PINCTRL_FUNC_SPI0_SS,
    PINCTRL_FUNC_SPI1_SS,
    PINCTRL_FUNC_SDIO0,
    PINCTRL_FUNC_SDIO0_PC,
    PINCTRL_FUNC_SDIO0_CD,
    PINCTRL_FUNC_SDIO0_WP,
    PINCTRL_FUNC_SDIO1,
    PINCTRL_FUNC_SDIO1_PC,
    PINCTRL_FUNC_SDIO1_CD,
    PINCTRL_FUNC_SDIO1_WP,
    PINCTRL_FUNC_NAND0,
    PINCTRL_FUNC_NAND0_CE,
    PINCTRL_FUNC_NAND0_RB,
    PINCTRL_FUNC_NAND0_DQS,
    PINCTRL_FUNC_TTC0_CLK,
    PINCTRL_FUNC_TTC0_WAV,
    PINCTRL_FUNC_TTC1_CLK,
    PINCTRL_FUNC_TTC1_WAV,
    PINCTRL_FUNC_TTC2_CLK,
    PINCTRL_FUNC_TTC2_WAV,
    PINCTRL_FUNC_TTC3_CLK,
    PINCTRL_FUNC_TTC3_WAV,
    PINCTRL_FUNC_UART0,
    PINCTRL_FUNC_UART1,
    PINCTRL_FUNC_USB0,
    PINCTRL_FUNC_USB1,
    PINCTRL_FUNC_SWDT0_CLK,
    PINCTRL_FUNC_SWDT0_RST,
    PINCTRL_FUNC_SWDT1_CLK,
    PINCTRL_FUNC_SWDT1_RST,
    PINCTRL_FUNC_PMU0,
    PINCTRL_FUNC_PCIE0,
    PINCTRL_FUNC_CSU0,
    PINCTRL_FUNC_DPAUX0,
    PINCTRL_FUNC_PJTAG0,
    PINCTRL_FUNC_TRACE0,
    PINCTRL_FUNC_TRACE0_CLK,
    PINCTRL_FUNC_TESTSCAN0,
};

enum XPmPinParam {
	PINCTRL_CONFIG_SLEW_RATE,
	PINCTRL_CONFIG_BIAS_STATUS,
	PINCTRL_CONFIG_PULL_CTRL,
	PINCTRL_CONFIG_SCHMITT_CMOS,
	PINCTRL_CONFIG_DRIVE_STRENGTH,
	PINCTRL_CONFIG_VOLTAGE_STATUS,
};

#ifdef __cplusplus
}
#endif

 /** @} */
#endif /* PM_DEFS_H_ */