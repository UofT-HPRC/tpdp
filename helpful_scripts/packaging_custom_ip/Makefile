# Edit the following four variables and run `make`.
# After that, follow the instructions in ip_maker.tcl
#
# Alternatively, you could set any of these variables on the command line:
#
#	$ make src_dir=/path/to/my/src dst_dir=/path/to/my/output
#
# Just make sure that you don't include a trailing slash on your directories

dst_dir=/path/to/my/output
src_dir=/path/to/my/src
ip_name=my_ip_name
part_no=xczu19eg-ffvc1760-2-i



# Makes Makefile easier to read
out_dir=${dst_dir}/${ip_name}

all: clean
	rm -rf ${out_dir}
	mkdir -p ${out_dir}/src
	cp $(shell find ${src_dir} -name "*.v" -o -name "*vh" -o -name "*sv") ${out_dir}/src
	# Remember to follow the instructions in ip_maker.tcl!
	vivado -nolog -nojournal -notrace -mode batch -source ip_maker.tcl -tclargs ${out_dir} ${ip_name} ${part_no}
	# Vivado leaves a bunch of crap laying around that no one cares about. So
	# delete it.
	rm -rf ${ip_name}_tmp_proj
	rm -f *log
	rm -rf .Xil
	rm -f vivado*
clean:
	rm -rf ${out_dir}
	rm -rf ${ip_name}_tmp_proj
	
