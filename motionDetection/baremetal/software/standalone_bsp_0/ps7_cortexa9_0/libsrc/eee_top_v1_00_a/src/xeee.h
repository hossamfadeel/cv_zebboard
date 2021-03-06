// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2013.3
// Copyright (C) 2013 Xilinx Inc. All rights reserved.
// 
// ==============================================================

#ifndef XEEE_H
#define XEEE_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xeee_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Axi_lite_para_BaseAddress;
} XEee_Config;
#endif

typedef struct {
    u32 Axi_lite_para_BaseAddress;
    u32 IsReady;
} XEee;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XEee_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XEee_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XEee_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XEee_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XEee_Initialize(XEee *InstancePtr, u16 DeviceId);
XEee_Config* XEee_LookupConfig(u16 DeviceId);
int XEee_CfgInitialize(XEee *InstancePtr, XEee_Config *ConfigPtr);
#else
int XEee_Initialize(XEee *InstancePtr, const char* InstanceName);
int XEee_Release(XEee *InstancePtr);
#endif

void XEee_Start(XEee *InstancePtr);
u32 XEee_IsDone(XEee *InstancePtr);
u32 XEee_IsIdle(XEee *InstancePtr);
u32 XEee_IsReady(XEee *InstancePtr);
void XEee_EnableAutoRestart(XEee *InstancePtr);
void XEee_DisableAutoRestart(XEee *InstancePtr);

void XEee_SetMatchcount_v(XEee *InstancePtr, u32 Data);
u32 XEee_GetMatchcount_v(XEee *InstancePtr);
void XEee_SetFaddr_v(XEee *InstancePtr, u32 Data);
u32 XEee_GetFaddr_v(XEee *InstancePtr);
void XEee_SetVecoutaddr_v(XEee *InstancePtr, u32 Data);
u32 XEee_GetVecoutaddr_v(XEee *InstancePtr);
void XEee_SetModeladdr_v(XEee *InstancePtr, u32 Data);
u32 XEee_GetModeladdr_v(XEee *InstancePtr);
void XEee_SetModelcount_v(XEee *InstancePtr, u32 Data);
u32 XEee_GetModelcount_v(XEee *InstancePtr);
void XEee_SetErrthres_v(XEee *InstancePtr, u32 Data);
u32 XEee_GetErrthres_v(XEee *InstancePtr);
void XEee_SetResultaddr_v(XEee *InstancePtr, u32 Data);
u32 XEee_GetResultaddr_v(XEee *InstancePtr);
void XEee_SetMode_v(XEee *InstancePtr, u32 Data);
u32 XEee_GetMode_v(XEee *InstancePtr);

void XEee_InterruptGlobalEnable(XEee *InstancePtr);
void XEee_InterruptGlobalDisable(XEee *InstancePtr);
void XEee_InterruptEnable(XEee *InstancePtr, u32 Mask);
void XEee_InterruptDisable(XEee *InstancePtr, u32 Mask);
void XEee_InterruptClear(XEee *InstancePtr, u32 Mask);
u32 XEee_InterruptGetEnabled(XEee *InstancePtr);
u32 XEee_InterruptGetStatus(XEee *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
