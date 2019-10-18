#!/bin/bash

# Copied from Clark's traffic generator project

# This for loop goes into the HLS-generated Verilog and gets rid of those damned
# "_V"s it adds all over the place. Add that to the list of things Vivado does
# that mostly just annoy you 

# Make sure you set the ip_name environment variable. This is done inside the main
# makefile

source config.mk

for i in "$ip_name/solution1/syn/verilog/*.v"; do
	sed -i 's/_V//g' $i
done
