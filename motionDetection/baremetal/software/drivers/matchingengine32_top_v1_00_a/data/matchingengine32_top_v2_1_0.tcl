# ==============================================================
# File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
# Version: 2013.3
# Copyright (C) 2013 Xilinx Inc. All rights reserved.
# 
# ==============================================================

proc generate {drv_handle} {
    xdefine_include_file $drv_handle "xparameters.h" "XMatchingengine32" \
        "NUM_INSTANCES" \
        "DEVICE_ID" \
        "C_S_AXI_SLV_PARA_BASEADDR" \
        "C_S_AXI_SLV_PARA_HIGHADDR"

    xdefine_config_file $drv_handle "xmatchingengine32_g.c" "XMatchingengine32" \
        "DEVICE_ID" \
        "C_S_AXI_SLV_PARA_BASEADDR"

    xdefine_canonical_xpars $drv_handle "xparameters.h" "XMatchingengine32" \
        "DEVICE_ID" \
        "C_S_AXI_SLV_PARA_BASEADDR" \
        "C_S_AXI_SLV_PARA_HIGHADDR"
}

