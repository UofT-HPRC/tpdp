`timescale 1ns / 1ps

module axis_swap_endian (
    input wire clk,
    input wire rst,
    
    input wire enable_swap,
    
    //For sonar (and Vivado) to properly use AXI Stream, must make sure
    //all wires in an interface have the form "name_TCHANNEL"
    input wire [31:0] old_TDATA,
    input wire [3:0] old_TKEEP,
    input wire old_TLAST,
    input wire old_TVALID,
    output wire old_TREADY,
    
    input wire [31:0] new_TDATA,
    input wire [3:0] new_TKEEP,
    input wire new_TLAST,
    input wire new_TVALID,
    output wire new_TREADY
);

    assign new_TDATA = enable_swap ?
        {old_TDATA[7:0], old_TDATA[15:8], old_TDATA[23:16], old_TDATA[31:24]}
        : old_TDATA
    ;
    
    assign new_TKEEP = enable_swap ?
        {old_TKEEP[0], old_TKEEP[1], old_TKEEP[2], old_TKEEP[3]}
        : old_TKEEP
    ;
    
    assign new_TLAST = old_TLAST;
    
    assign old_TREADY = new_TREADY;

endmodule
