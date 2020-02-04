# The PC Group Documentation Project

Yes, we have a wiki. The reason for this repo is because I want to spend some 
time explaining source code as well as uploading helpful bash/TCL scripts

# Other related repos:

https://github.com/UofT-HPRC/mpsoc_drivers


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
   100G ethernet for the MPSoCs.
 - The Zynq presets for the MPSoC

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

```
├── bugs_and_fixes_and_workarounds_oh_my
│   ├── 100g_headaches.txt
│   ├── axi_interconnect_woes.txt
│   ├── debug_bridge_anguish.txt
│   ├── dma_sorrow.txt
│   ├── mpsoc_blues.txt
│   ├── other_axi_troubles.txt
│   ├── README
│   └── vivado_gotchas.txt
├── configuring_server
│   ├── 100g_switch
│   │   └── 100g_switch.csv
│   ├── 100m_switch
│   │   └── 100m_switch.csv
│   ├── 10g_switch
│   │   └── 10g_switch.csv
│   ├── 1g_switch
│   │   └── 1g_switch.csv
│   └── README
├── galapagos
│   ├── adding_new_vivado_version.txt
│   └── moving_parts.txt
├── helpful_scripts
│   ├── how_clark_compiles_hls
│   │   ├── config.mk
│   │   ├── include
│   │   │   └── PUT_YOUR_HEADER_FILES_HERE
│   │   ├── Makefile
│   │   ├── README
│   │   ├── scripts
│   │   │   ├── clean_names.sh
│   │   │   ├── hls.tcl
│   │   │   ├── ip_package.tcl
│   │   │   └── pick_up_vivados_dirty_socks.sh
│   │   └── src
│   │       └── PUT_YOUR_SOURCE_FILES_HERE
│   ├── mpsoc_bash_stuff
│   │   ├── camilos_aliases
│   │   ├── clarks_aliases
│   │   ├── disablePci.sh
│   │   ├── enableATB.sh
│   │   ├── marcos_aliases
│   │   └── README
│   ├── packaging_custom_ip
│   │   ├── ip_maker.tcl
│   │   ├── Makefile
│   │   └── README.txt
│   └── tcl_utilities
│       ├── 100g_qsfp0.tcl
│       ├── 100g_qsfp1.tcl
│       └── ps_set_sidewinder.tcl
├── hls_stuff
│   ├── quick_and_dirty_interfaces
│   │   ├── axilite_reg_intf.cpp
│   │   └── quick_and_dirty_interfaces.txt
│   └── README.txt
├── ip_core_tidbits
│   ├── axi_dma.txt
│   ├── axi_gpio.txt
│   ├── ddr4_mig.txt
│   ├── gulf_stream_and_lbus_converters.txt
│   ├── usplus_100g.txt
│   └── xdma.txt
├── linux_kernel_programming
│   ├── basic_driver.c
│   ├── basic_memory_management_and_mmap.txt
│   ├── compiling_mpsoc_modules.txt
│   ├── device_tree_overlays.txt
│   ├── driver_basics.txt
│   ├── idioms.txt
│   ├── interrupt_numbers.txt
│   ├── README
│   └── sysfs_tidbits.txt
├── lwip
│   └── the_limit_of_marcos_lwip_knowledge.txt
├── misc
│   ├── flex_bison_c++.txt
│   ├── setting_up_lab_internet.txt
│   ├── sockets.txt
│   ├── tips_and_tricks.txt
│   └── vivado_with_git.txt
├── mpsoc_programming
│   ├── drivers
│   │   ├── mpsoc_axiregs
│   │   │   ├── commentary
│   │   │   ├── mpsoc_axiregs.c
│   │   │   └── README
│   │   ├── mpsoc_PSRegs
│   │   │   ├── commentary
│   │   │   └── mpsoc_PSRegs.c
│   │   ├── pinner
│   │   │   ├── commentary.c
│   │   │   ├── pinner.c
│   │   │   ├── pinner.h
│   │   │   └── pinner_private.h
│   │   ├── uio
│   │   │   ├── axitimer_module.c
│   │   │   ├── axitimer_user.c
│   │   │   └── writing_uio_drivers.txt
│   │   └── uio_with_sysfs
│   │       ├── axidma_module.c
│   │       ├── interrupt_numbers_refresher.txt
│   │       ├── putting_it_together.txt
│   │       ├── README.txt
│   │       ├── sysfs_attribute_files.txt
│   │       └── uio_refresher.txt
│   ├── how_to_use_marcos_drivers
│   │   ├── axidma
│   │   │   ├── pinner.h
│   │   │   ├── README.txt
│   │   │   └── user_example.c
│   │   ├── axidma_userlib
│   │   └── pinner
│   │       ├── pinner.h
│   │       ├── README.txt
│   │       └── userspace_example.c
│   ├── old_and_crotchety_ps_to_pl.txt
│   ├── poke
│   │   ├── dpoke.c
│   │   ├── poke.c
│   │   └── README
│   ├── ps_to_pl
│   │   ├── include
│   │   │   ├── axidma_ctrl_func.h
│   │   │   ├── axidma_ioctl.h
│   │   │   ├── conversion.h
│   │   │   ├── libaxidma.h
│   │   │   └── util.h
│   │   ├── README
│   │   ├── simple_register_accesses.txt
│   │   ├── stream_and_dma.txt
│   │   └── util
│   │       ├── axidma_ctrl_func.c
│   │       └── libaxidma.c
│   ├── sshing_into_the_mpsoc.txt
│   ├── the_onus_of_programming_the_mpsoc.txt
│   └── xvcserver
│       ├── README
│       └── xvcserver_axi
│           ├── include
│           │   ├── xvc_axi2pl.h
│           │   └── xvcserver.h
│           ├── Makefile
│           ├── output
│           │   └── xvcserver
│           └── src
│               ├── xvc_axi2pl.cpp
│               ├── xvc_server2axi.c
│               └── xvc_server.c
├── README.md
└── simulation_idioms
    ├── general_method
    │   ├── general_method.txt
    │   └── using_axi_vip.txt
    ├── how_marco_writes_testbenches
    │   ├── axistream_forwarder_drivers.mem
    │   ├── axistream_forwarder_tb.sv
    │   └── how_marco_writes_testbenches.txt
    ├── quick_and_dirty_method
    │   └── quick_and_dirty_sims.txt
    ├── using_icarus_verilog
    │   └── iverilog.txt
    ├── using_sonar
    └── vpi
        ├── general_vpi.txt
        ├── tutorial.txt
        └── vpi_core_api.txt
```
