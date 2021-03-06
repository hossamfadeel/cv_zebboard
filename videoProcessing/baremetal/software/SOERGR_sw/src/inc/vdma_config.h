/*
 * vdma_config.h
 *
 */

#ifndef VDMA_CONFIG_H_
#define VDMA_CONFIG_H_

#include "xaxivdma_hw.h"

#define AXI_FILTER_RX_CTRL 					XAXIVDMA_RX_OFFSET + XAXIVDMA_CR_OFFSET
#define AXI_FILTER_RX_START1 				XAXIVDMA_S2MM_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET
#define AXI_FILTER_RX_START2 				XAXIVDMA_S2MM_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET + 0x4
#define AXI_FILTER_RX_START3 				XAXIVDMA_S2MM_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET + 0x8
#define AXI_FILTER_RX_FRMDLY_STRIDE 		XAXIVDMA_S2MM_ADDR_OFFSET + XAXIVDMA_STRD_FRMDLY_OFFSET
#define AXI_FILTER_RX_HSIZE 				XAXIVDMA_S2MM_ADDR_OFFSET + XAXIVDMA_HSIZE_OFFSET
#define AXI_FILTER_RX_VSIZE 				XAXIVDMA_S2MM_ADDR_OFFSET + XAXIVDMA_VSIZE_OFFSET

#define AXI_FILTER_TX_CTRL 					XAXIVDMA_TX_OFFSET + XAXIVDMA_CR_OFFSET
#define AXI_FILTER_TX_START1 				XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET
#define AXI_FILTER_TX_START2 				XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET + 0x4
#define AXI_FILTER_TX_START3 				XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET + 0x8
#define AXI_FILTER_TX_FRMDLY_STRIDE 		XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_STRD_FRMDLY_OFFSET
#define AXI_FILTER_TX_HSIZE 				XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_HSIZE_OFFSET
#define AXI_FILTER_TX_VSIZE 				XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_VSIZE_OFFSET

enum dma_transfer_direction {
	DMA_MEM_TO_MEM,
	DMA_MEM_TO_DEV,
	DMA_DEV_TO_MEM,
	DMA_DEV_TO_DEV,
	DMA_TRANS_NONE,
};


void config_filterVDMA(int VDMA_baseAddr, int vdma_direction, unsigned long base_address, unsigned short horizontalActiveTime, unsigned short verticalActiveTime);
void resetVDMA(int VDMA_baseAddr);

#endif /* VDMA_CONFIG_H_ */
