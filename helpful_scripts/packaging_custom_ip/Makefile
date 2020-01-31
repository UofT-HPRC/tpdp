# This creates a directory named after the desired IP in the current folder.
# Edit the following three variables and run `make`.
# After that, follow the instructions in ip_maker.tcl

src_dir=./src
ip_name=axistreamstats
part_no=xczu19eg-ffvc1760-2-i

# Makes Makefile easier to read
out_dir=${ip_name}

all: clean
	rm -rf ${ip_name}
	mkdir -p ${ip_name}/src
	cp $(shell find ${src_dir} -name "*.v" -o -name "*vh" -o -name "*sv") ${out_dir}/src
	vivado -nolog -nojournal -notrace -mode batch -source ip_maker.tcl -tclargs ${ip_name} ${part_no}
	# Vivado leaves a bunch of crap laying around that no one cares about. So
	# delete it.
	rm -rf ${ip_name}_tmp_proj
	rm -f *log
	rm -rf .Xil
	rm -f vivado*
clean:
	rm -rf ${ip_name}
	rm -rf ${ip_name}_tmp_proj
	
