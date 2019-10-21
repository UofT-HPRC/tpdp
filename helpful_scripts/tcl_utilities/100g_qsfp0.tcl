# In Vivado, do Tools -> Run Tcl Script... and run this file

# It will do a lot of the annoying grunt work needed to set up the 100G
# core on QSFP0 o nthe MPSoC

# I got this by editing the design in the block diagram editor and 
# copying out the commands Vivado issued. It's a good feature, I only
# wish it would work more often

create_bd_cell -type hier QSFP0
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:cmac_usplus:2.5 QSFP0/cmac_usplus_0
endgroup
set_property -dict [list CONFIG.CMAC_CAUI4_MODE {1} CONFIG.NUM_LANES {4} CONFIG.GT_REF_CLK_FREQ {322.265625} CONFIG.GT_DRP_CLK {200.00} CONFIG.RX_CHECK_PREAMBLE {1} CONFIG.RX_CHECK_SFD {1} CONFIG.TX_FLOW_CONTROL {0} CONFIG.RX_FLOW_CONTROL {0} CONFIG.ENABLE_AXI_INTERFACE {0} CONFIG.CMAC_CORE_SELECT {CMACE4_X0Y1} CONFIG.GT_GROUP_SELECT {X0Y12~X0Y15} CONFIG.LANE1_GT_LOC {X0Y12} CONFIG.LANE2_GT_LOC {X0Y13} CONFIG.LANE3_GT_LOC {X0Y14} CONFIG.LANE4_GT_LOC {X0Y15} CONFIG.LANE5_GT_LOC {NA} CONFIG.LANE6_GT_LOC {NA} CONFIG.LANE7_GT_LOC {NA} CONFIG.LANE8_GT_LOC {NA} CONFIG.LANE9_GT_LOC {NA} CONFIG.LANE10_GT_LOC {NA}] [get_bd_cells QSFP0/cmac_usplus_0]
startgroup
create_bd_intf_pin -mode Slave -vlnv xilinx.com:display_cmac_usplus:lbus_ports:2.0 QSFP0/lbus_tx
connect_bd_intf_net [get_bd_intf_pins QSFP0/lbus_tx] [get_bd_intf_pins QSFP0/cmac_usplus_0/lbus_tx]
endgroup
startgroup
create_bd_intf_pin -mode Master -vlnv xilinx.com:display_cmac_usplus:lbus_ports:2.0 QSFP0/lbus_rx
connect_bd_intf_net [get_bd_intf_pins QSFP0/lbus_rx] [get_bd_intf_pins QSFP0/cmac_usplus_0/lbus_rx]
endgroup
startgroup
create_bd_pin -dir I QSFP0/init_clk
connect_bd_net [get_bd_pins QSFP0/init_clk] [get_bd_pins QSFP0/cmac_usplus_0/init_clk]
endgroup
startgroup
create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 QSFP0/gt_ref_clk
connect_bd_intf_net [get_bd_intf_pins QSFP0/gt_ref_clk] [get_bd_intf_pins QSFP0/cmac_usplus_0/gt_ref_clk]
endgroup
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 QSFP0/xlconstant_0
endgroup
set_property name one [get_bd_cells QSFP0/xlconstant_0]
set_property location {1 149 -46} [get_bd_cells QSFP0/one]
connect_bd_net [get_bd_pins QSFP0/cmac_usplus_0/ctl_tx_enable] [get_bd_pins QSFP0/one/dout]
connect_bd_net [get_bd_pins QSFP0/cmac_usplus_0/ctl_rx_enable] [get_bd_pins QSFP0/one/dout]
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 QSFP0/xlconstant_0
endgroup
set_property name zero [get_bd_cells QSFP0/xlconstant_0]
set_property -dict [list CONFIG.CONST_VAL {0}] [get_bd_cells QSFP0/zero]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/ctl_tx_test_pattern]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/drp_clk]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/core_drp_reset]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/core_tx_reset]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/core_rx_reset]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/sys_reset]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/gtwiz_reset_rx_datapath]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/gtwiz_reset_tx_datapath]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/ctl_tx_send_idle]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/ctl_tx_send_lfi]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/ctl_tx_send_rfi]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/ctl_rx_force_resync]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/ctl_rx_test_pattern]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/drp_en]
connect_bd_net [get_bd_pins QSFP0/zero/dout] [get_bd_pins QSFP0/cmac_usplus_0/drp_we]
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 QSFP0/xlconstant_0
endgroup
set_property name zero_x10 [get_bd_cells QSFP0/xlconstant_0]
connect_bd_net [get_bd_pins QSFP0/zero_x10/dout] [get_bd_pins QSFP0/cmac_usplus_0/drp_addr]
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 QSFP0/xlconstant_0
endgroup
set_property name zero_x12 [get_bd_cells QSFP0/xlconstant_0]
connect_bd_net [get_bd_pins QSFP0/zero_x12/dout] [get_bd_pins QSFP0/cmac_usplus_0/gt_loopback_in]
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 QSFP0/xlconstant_0
endgroup
set_property name zero_x16 [get_bd_cells QSFP0/xlconstant_0]
connect_bd_net [get_bd_pins QSFP0/zero_x16/dout] [get_bd_pins QSFP0/cmac_usplus_0/drp_di]
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 QSFP0/xlconstant_0
endgroup
set_property name zero_x56 [get_bd_cells QSFP0/xlconstant_0]
connect_bd_net [get_bd_pins QSFP0/zero_x56/dout] [get_bd_pins QSFP0/cmac_usplus_0/tx_preamblein]
startgroup
set_property -dict [list CONFIG.CONST_WIDTH {10} CONFIG.CONST_VAL {0}] [get_bd_cells QSFP0/zero_x10]
endgroup
startgroup
set_property -dict [list CONFIG.CONST_WIDTH {12} CONFIG.CONST_VAL {0}] [get_bd_cells QSFP0/zero_x12]
endgroup
startgroup
set_property -dict [list CONFIG.CONST_WIDTH {16} CONFIG.CONST_VAL {0}] [get_bd_cells QSFP0/zero_x16]
endgroup
startgroup
set_property -dict [list CONFIG.CONST_WIDTH {56} CONFIG.CONST_VAL {0}] [get_bd_cells QSFP0/zero_x56]
endgroup
startgroup
create_bd_pin -dir O QSFP0/qsfp0_clk
connect_bd_net [get_bd_pins QSFP0/qsfp0_clk] [get_bd_pins QSFP0/cmac_usplus_0/gt_txusrclk2]
endgroup
connect_bd_net [get_bd_pins QSFP0/cmac_usplus_0/rx_clk] [get_bd_pins QSFP0/cmac_usplus_0/gt_txusrclk2]


