This is a Makefile that helps you package a Verilog project as a Vivado IP. If 
you want the packaged IP to end up in /my/output/dir, and you have a bunch of 
Verilog files in /my/src/dir

    1. Copy this Makefile and ip_maker.tcl into /my/output/dir
    2. Edit the copied Makefile's src_dir variable to be /my/src/dir
        -> This can be any directory, but please note that Vivado will scan
           the source directory recursively and pull out all files that end
           in ".v", ".sv", or ".vh"
    3. Set the part name and desired IP name in the Makefile
    2. Run make
    
That should just work, but if it doesn't, feel free to contact me
