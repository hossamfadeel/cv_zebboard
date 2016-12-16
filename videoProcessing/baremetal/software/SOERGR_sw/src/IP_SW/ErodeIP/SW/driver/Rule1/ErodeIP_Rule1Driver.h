// this driver corresponds to Grayscale IP configuration corresponding to GRIP Rule 1 i.e Grayscale IP integrated with AXI-VDMA 
// it is generated by modifying the auto-generated template


#ifndef __ERODEIP_RULE1DRIVER_H__
#define __ERODEIP_RULE1DRIVER_H__


#include "ErodeIP_VDMA_Driver.h"
#include "xscugic.h"

#define bool unsigned char


// all the hardware related info needed by the driver functions should be provided in this struct
typedef struct {
    unsigned int baseaddr;
    ERODEIP_VDMADriverInstance vdmaDriver;
} ErodeIPRule1DriverInstance;


typedef struct {
    unsigned int offset;
    unsigned int value;
    unsigned int mask;
} ErodeIPRule1RegType;



// register map for Grayscale IP peripheral registers
typedef struct {
	ErodeIPRule1RegType AP_CTRL;
	ErodeIPRule1RegType GIE;
	ErodeIPRule1RegType IER;
	ErodeIPRule1RegType ISR;
	ErodeIPRule1RegType ROWS_DATA;
	ErodeIPRule1RegType COLS_DATA;
} ErodeIPRule1RegMap;



// API for GrayscaleIP_Driver to use if this rule is applied by GRIP
void ErodeIP_Rule1Driver_initialize(ErodeIPRule1DriverInstance *InstancePtr, XScuGic *InterruptController, unsigned long ImgIn_BaseAddr,unsigned long ImgOut_BaseAddr, unsigned short width, unsigned short height, unsigned short horizontalActiveTime, unsigned short verticalActiveTime);
void ErodeIP_Rule1Driver_start(ErodeIPRule1DriverInstance *InstancePtr, unsigned long ImgIn_BaseAddr,unsigned long ImgOut_BaseAddr, unsigned short width, unsigned short height, unsigned short horizontalActiveTime, unsigned short verticalActiveTime);
void ErodeIP_Rule1Driver_stop(ErodeIPRule1DriverInstance *InstancePtr);
bool ErodeIP_Rule1Driver_isBusy(ErodeIPRule1DriverInstance *InstancePtr);

#endif









