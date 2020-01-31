This is a Makefile that helps you package a Verilog project as a Vivado IP. If 
you want the packaged IP to end up in /my/output/dir, and you have a bunch of 
Verilog files in /my/src/dir

    1. Edit the copied Makefile's dst_dir variable to be /my/output/dir
    2. Edit the copied Makefile's src_dir variable to be /my/src/dir
        -> This can be any directory, but please note that Vivado will scan
           the source directory recursively and pull out all files that end
           in ".v", ".sv", or ".vh"
    3. Set the ip_name and part_no to be the desired IP name and part number, 
       respectively.
    2. Run make
    
That should just work, but if it doesn't, feel free to contact me


By the way, you can call this makefile with arguments:

    $ make src_dir=/path/to/my/src dst_dir=/path/to/my/output ip_name=my_ip_name
