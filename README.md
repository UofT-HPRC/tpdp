# The PC Group Documentation Project

Yes, we have a wiki. The reason for having this repo is because to spend a 
little time explaining source code as well as uploading helpful bash scripts

# Contents

Right now things are a little unorganized while I figure out how to put it all
together.

Right now I have this large text file (`the_trials_and_tribulations_of_programming_the_mpsoc.txt`)
that keeps growing and growing. Some of it references source code in other 
folders.

`tips_and_tricks.txt` just lists a bunch of quick things that I use often, and
could help you save time when doing work.

`configuring server` will one day contain 
 - Instructions for configuring switches
 - Documentation on our different subnets
 - Connection diagrams
 - How containers are set up
 - Setting up internet for new students
 - etc
I don't really know how to do any of this, so I'll fill these in as I learn them

`helpful_scripts` right now contains some bach aliases used on the MPSoCs, as 
well as a neat little method Clark uses to compile HLS (I really like it)

`linux_kernel_programming` will contain just a few tips and pointers that will
make it easier to understand Linux kernel source code, and potentially help you
write drivers.

`lwip` right now contains my personal "internal documentation" I wrote for using
LWIP, and as soon as I track it down I'll put Daniel's much better tutorial in 
there too.

`MPSoC_sources` contains some driver code we use often, the code for the XVC
server, and some example of PS to PL communication.

`simulation_idioms` contains some tutorial-style material to help you:
 - Quickly throw together testbenches for HDL wrappers 
 - Use the AXI VIP
 - A little method I use to make it easier to define simulation inputs
 - A much better method Varun uses to auto-generate entire simulation rigs
 

Well, not all of these are filled up yet, but I guess I'm working on it.
