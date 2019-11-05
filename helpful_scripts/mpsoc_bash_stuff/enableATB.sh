#!/bin/bash

# Hopefully, this enables the AXI timeout blocks (ATB) in the ARM and we can prevent hangs!!!

# This information found in the Zynq MPSoC technical reference manual (UG1085) in chapter 15
# (the PS Interconnect chapter).
# I also had to reference the register maps, which can be browsed here:
# https://www.xilinx.com/html_docs/registers/ug1087/ug1087-zynq-ultrascale-registers.html

# There are two domains for AXI PS Masters, Full Power Domain (FPD) and Low Power Domain (LPD)
# These variables are the most significant 16 bits of the address for the control registers
FPD=0xFD61
LPD=0xFF41

# These are the least significant 16 buts of the addresses for the control registers we'll use
RESP_EN=6014
RESP_TYPE=6018
PRESCALE=6020
CMD_STORE_EN=6010

# The FPD has three ATBs. This is why we set some registers to '7': one '1' for each ATB

# Disable responses while we change the response type
devmem2 $FPD$RESP_EN b 0

# Set response type to "issue an error" mode for all three ATBs in the Full Power Domain
devmem2 $FPD$RESP_TYPE b 7

# Re-enable responses (note: this does not enable the ATB in general)
devmem2 $FPD$RESP_EN b 7

# Enable the clock, and set the prescale value to FFFF
devmem2 $FPD$PRESCALE w 0x10100

# Actually enable the ATB modules
devmem2 $FPD$CMD_STORE_EN b 7


# The LPD has two ATBs. This is why we use '3' instead of '7'
devmem2 $LPD$RESP_EN b 0
devmem2 $LPD$RESP_TYPE b 3
devmem2 $LPD$RESP_EN b 3
devmem2 $LPD$PRESCALE w 0x10100
devmem2 $LPD$CMD_STORE_EN b 3

