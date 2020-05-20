#!/bin/bash

# Little helper script to automate the archiving and renaming I do when 
# I make a new bitstream

# HOW TO USE:

# Copy newly generated .bit and .ltx files to this folder with a new 
# name (i.e. different from any .bit or .ltx files that are already 
# here). Then run this script. This will do the right thing whether or
# not you have old bitstreams laying around

# Don't forget to write yourself a helpful text file!

if [ -e *.bin ]; then
	old_name=$(ls *.bin | cut -d "." -f 1)
	old_name_in_bif=$(grep -o -P " \K([^ ]*)(\.)" *.bif | cut -d "." -f 1)
	new_name=$(ls *.bit | grep -v $old_name | cut -d "." -f 1)

	sed -e "s/$old_name_in_bif/$new_name/g" *.bif > $new_name.bif

	mkdir -p old/$old_name
	mv $old_name.{bit,bin,bif} old/$old_name/
	if [ -e $old_name.ltx ]; then
		mv $old_name.ltx old/$old_name/
	fi
	
	if [ -e $old_name.txt ]; then 
		cp $old_name.txt old/$old_name/
		mv $old_name.txt $new_name.txt
		touch TXT_NOT_UPDATED
		echo "Remember to edit your .txt file; when done, delete the TXT_NOT_UPDATED file"
	else
		echo "It is highly recommended to include a text file explaining this bitstream!!!!!!!!!"
	fi
else
	echo "Generating a new .bif file..."
	if [ ! -e *.bit ]; then
		echo "Must supply a bitstream"
		exit 1
	fi
	
	bit_name=$(ls *.bit | cut -d "." -f 1)
	
	cat <<EOF > $bit_name.bif
all:
{
        [destination_device = pl] $bit_name.bit
}
EOF
	
	if [ ! -e *.txt ]; then
		echo "It is highly recommended to include a text file explaining this bitstream!!!!!!!!!"
	fi
fi

source $(locate "settings64.sh" | grep Vivado)

bin_new_name=$(ls *.bif | cut -d "." -f 1)
bootgen -arch zynqmp -image $bin_new_name.bif -o $bin_new_name.bin -w

