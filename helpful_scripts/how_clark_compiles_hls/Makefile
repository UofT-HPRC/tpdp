# Copied from Clark's traffic generator project

include config.mk
# config.mk includes all the environment variables used by the scripts in the
# scripts folder. It was copied from Clark's traffic generator project and then
# modified to match current project

all: clean
	# Generate the HLS IP
	vivado_hls scripts/hls.tcl $(ip_name) $(part_name) $(period)
	# Get rid of pesky "_V"s
	scripts/clean_names.sh
	# Package IP nicely using Vivado
	vivado -mode tcl -nolog -nojournal -source scripts/ip_package.tcl -tclargs $(ip_name) $(part_name)
	# Clean up after Vivado...
	scripts/pick_up_vivados_dirty_socks.sh
	# Not 100% sure what Clark was usign this for
	touch .timestamp
clean:
	rm -rf $(ip_name)
	rm -rf $(ip_name)_ip
