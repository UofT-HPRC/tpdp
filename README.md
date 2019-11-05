# The PC Group Documentation Project

Yes, we have a wiki. The reason for this repo is because I want to spend some 
little time explaining source code as well as uploading helpful bash scripts

# Contents

Right now things are a little unorganized while I figure out how to put it all
together.

A very important thing to do in the near future is add a nice index to our 
interal docs. Often, one of us has written information on doing certain things, 
but no one else knows it exists or how to get it. For now, please reference 
this crappy list:

`bugs_and_fixes_and_workarounds_oh_my` contains exactly what you think it 
contains. Please add your own discoveries!

`configuring server` will one day contain 
 - Instructions for configuring switches
 - Documentation on our different subnets
 - Connection diagrams
 - How containers are set up
 - Setting up internet for new students
 - etc
I don't really know how to do any of this, so I'll fill these in as I learn them

`helpful_scripts` right now contains:
 - bash aliases used on the MPSoCs, and a script to enable AXI timeout checking
 - A neat little method Clark uses to compile HLS (I really like it). 
 - Some tcl utilities. For example, scripts that automatically configure the 
   100G ethernet for the MPSoCs

`ip_core_tidbits` is just meant to help you get started using common IPs. Right 
now we have stuff on:
 - GULF Stream and the lbus converters
 - Setting up the 100G ethernet subsystem

`linux_kernel_programming` will contain just a few tips and pointers that will 
make it easier to understand Linux kernel source code, and potentially help you 
write drivers. Note that there is some MPSoC-specific stuff in here, and a 
little bit of info on device trees.

`lwip` right now contains my personal "internal documentation" I wrote for using
LWIP, and as soon as I track it down I'll put Daniel's much better tutorial in 
there too.

`misc` contains my personal list of tips and tricks, as well as a short file to 
explain how to get flex and bison to work in C++.

`mpsoc_programming` is a quickly-growing folder of the many many things you 
need to know to develop on the MPSoC boards.
 - I have this large text file  (`the_trials_and_tribulations_of_programming_the_mpsoc.txt`) 
   that just keeps getting bigger. Long story short: getting a bitstream onto an
   MPSoC is quite challenging!
 - I also took the liberty of trying to explain the source code in the drivers
   that Clark put together. These drivers are needed by a lot of things.
 - Finally, there's some detailed API documentation for the PS-to-PL libraries 
   that Clark and Camilo wrote

`simulation_idioms` contains some tutorial-style material to help you:
 - Quickly do basic simulations without having to write a testbench
 - Quickly throw together testbenches for HDL wrappers 
 - Use the AXI VIP
 - A little method I use to make it easier to define simulation inputs
 - A much better method Varun uses to auto-generate entire simulation rigs
 
More to come...
