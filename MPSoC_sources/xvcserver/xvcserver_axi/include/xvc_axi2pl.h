#define LENGTH_REG_OFFSET	0x00
#define TMS_REG_OFFSET		0x04 
#define TDI_REG_OFFSET		0x08
#define TDO_REG_OFFSET		0x0C
#define CONTROL_REG_OFFSET	0x10
typedef unsigned int u32;
struct xil_xvc_ioc {
        u32 opcode;
        u32 length;
        unsigned char *tms_buf;
        unsigned char *tdi_buf;
        unsigned char *tdo_buf;
};
int xvc_axiioc(volatile void* axi_map_base, xil_xvc_ioc * xvc_obj);
