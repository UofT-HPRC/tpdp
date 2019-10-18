#include <iostream>
#include <stdlib.h>
#include <cstring>
#include "xvc_axi2pl.h"

using namespace std;

u32 axilite_read(volatile void* map_base, int offset) {
	volatile void* virt_addr = (volatile void*)((char*)map_base + offset);
	u32 value = *((u32 *) virt_addr);
	return value;
}

void axilite_write(volatile void* map_base, int offset, int value) {
	volatile void* virt_addr = (volatile void*)((char*)map_base + offset);
	*((u32 *) virt_addr) = value;
}

int xil_xvc_shift_bits(volatile void* map_base, u32 tms_bits, u32 tdi_bits, u32 *tdo_bits) {
	u32 control_reg_data = 1;
	int count = 100;

	// Set tms bits
	axilite_write(map_base, TMS_REG_OFFSET, tms_bits);

	// Set tdi bits and shift data out
	axilite_write(map_base, TDI_REG_OFFSET, tdi_bits);
	// Enable shift operation
	axilite_write(map_base, CONTROL_REG_OFFSET, 0x01);

	// poll status to wait for completion

	while (count) {
		// Read control reg to check shift operation completion
		control_reg_data = axilite_read(map_base, CONTROL_REG_OFFSET);
		if ((control_reg_data & 0x01) == 0)     {
	   		break;
		}
		count--;
	}
	if (count == 0) {
		cout << "XVC AXI transaction timed out" << endl;
		return 1;
	}
	// Read tdo bits back out
	*tdo_bits = axilite_read(map_base, TDO_REG_OFFSET);
	return 0;
}

int xvc_axiioc(
	volatile void* axi_map_base,
	xil_xvc_ioc * xvc_obj
) {
	u32 operation_code;
	u32 num_bits;
	int num_bytes;
	int current_bit;
	u32 bypass_status;
	int ret = 0;

	operation_code = xvc_obj->opcode;
	if (operation_code != 0x01 && operation_code != 0x02) {
		ret = 1;
		return ret;
	}
	num_bits = xvc_obj->length;
	num_bytes = (num_bits + 7) / 8;
	if (operation_code == 0x2) {
		bypass_status = 0x2;
	} else {
		bypass_status = 0x0;
	}
	if (num_bits >= 32) {
		axilite_write(axi_map_base, LENGTH_REG_OFFSET, 0x20);
	}
	current_bit = 0;
	while (current_bit < num_bits) {
		int shift_num_bytes;
		int shift_num_bits = 32;

		u32 tms_store = 0;
		u32 tdi_store = 0;
		u32 tdo_store = 0;

		if (num_bits - current_bit < shift_num_bits) {
			shift_num_bits = num_bits - current_bit;
			axilite_write(axi_map_base, LENGTH_REG_OFFSET, shift_num_bits);
		}

		shift_num_bytes = (shift_num_bits + 7) / 8;

		memcpy(&tms_store, xvc_obj->tms_buf + (current_bit / 8), shift_num_bytes);
		memcpy(&tdi_store, xvc_obj->tdi_buf + (current_bit / 8), shift_num_bytes);

		ret = xil_xvc_shift_bits(axi_map_base, tms_store, tdi_store, &tdo_store);
		if (ret) {
			goto cleanup;
		}

		memcpy(xvc_obj->tdo_buf + (current_bit / 8), &tdo_store, shift_num_bytes);
		current_bit += shift_num_bits;
	}
	if (bypass_status) {
		memcpy(xvc_obj->tdo_buf, xvc_obj->tdi_buf, num_bytes);
	}
cleanup:
	return ret;
}
