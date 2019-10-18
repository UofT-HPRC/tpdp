#!/bin/bash

# This file is used by overlay-program. Basically, it is very unsafe for any
# device to be using the FPGA when you are applying a new device tree overlay,
# so we prohylactically disable everything

arrayInsert() {
	Array=($1)
	NewEle=$2
	N=$3
	OutArray=( ${Array[@]:0:$N} $NewEle ${Array[@]:$N} )
	echo ${OutArray[@]}
}
pciDeviceHeap=()
baseLocation="../../../devices/platform/amba/.*\.axi-pcie/pci[0-9]{4}:[0-9]{2}/"
pciDevices=(`find /sys/bus/pci/devices/ -mindepth 1 -maxdepth 1`)
for pciDevice in ${pciDevices[@]}; do
	pciDeviceAbusLoc=`readlink -f $pciDevice`
	pciDeviceLoc=`readlink $pciDevice`
	pciDeviceBaseLoc=`echo $pciDeviceLoc | sed -r "s|($baseLocation).*|\1|"`
	pciDeviceRelaLoc=`echo $pciDeviceLoc | sed -r "s|$baseLocation||"`
	tmpVar=${pciDeviceRelaLoc//[^\/]}
	depth=${#tmpVar}
	let i=0
	for ele in ${pciDeviceHeap[@]}; do
		currentDepth=${ele#*,}
		if [ $currentDepth -lt $depth ]; then
			break
		fi
		let i++
	done
	if [ ${#pciDeviceHeap[@]} -eq 0 ]; then
		pciDeviceHeap=( "$pciDeviceAbusLoc,$depth" )
	else
		pciDeviceHeap=(`arrayInsert "${pciDeviceHeap[@]}" "$pciDeviceAbusLoc,$depth" $i`)
	fi
done

maxIndex=`expr ${#pciDeviceHeap[@]} - 1`
for i in `seq 0 $maxIndex`; do
	pciDeviceHeap[$i]=${pciDeviceHeap[$i]%,*}
#	echo "echo 0 > ${pciDeviceHeap[$i]}/enable"
#	echo 0 > ${pciDeviceHeap[$i]}/enable
	echo "echo 1 > ${pciDeviceHeap[$i]}/remove"
	echo 1 > ${pciDeviceHeap[$i]}/remove
done
#echo 1 > /sys/bus/pci/rescan
