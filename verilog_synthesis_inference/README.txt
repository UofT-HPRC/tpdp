When writing designs for FPGAs, sometimes you need to know how your Verilog 
code will be synthesized by Vivado. This can make your designs much more area 
efficient (and have faster synthesis/P&R times!) for almost no extra 
development time.

Charles points out that Ken Chapman has a number of Xilinx whitepapers that 
talk about exactly this; taking full advantage of FPGA resources. One such 
example is

https://www.xilinx.com/support/documentation/white_papers/wp271.pdf

which shows how to use a LUT as a 16-bit shift register (much more efficient 
than using 16 FFs!).

As I learn more of these techniques, I'll put them in separate files in this 
folder.
