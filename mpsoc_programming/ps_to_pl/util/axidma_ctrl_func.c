#include "axidma_ctrl_func.h"
#include "libaxidma.h"
#include <assert.h>
bool non_sudo_fd_initialized = false;
bool non_sudo_fd_used = false;
int non_sudo_fd = 0;
bool fd_initialized=false;
int fd=0;
bool clk_initialized = false;
struct axilite_obj clk;
int pl_clk_config(bool start, int clk_number, int divisor_1, int divisor_2, int type)
{
	//verify the type and the clk number to see if input is valid
	if (type>3 || type < 0 || type==1 || clk_number > 3 || clk_number < 0)
	{
		return -1;
	}
	//calculate the offset within the PL IP object
	unsigned int offset = 0xC0 + clk_number *4;
	//initialize a variable to hold the value
	unsigned int value = 0;
	if (start)
	{
		//Byte 0 = type, Byte 1 = divisor_2, Byte 2 = divisor_1, Byte 0 = Enable(1) or Disable (1)
		value = (1<<24)+divisor_1<<16+divisor_2<<8+type;
		//If start = false, value=0 which disables the clock
	}
	if (!clk_initialized)
	{
		//Initialize an IP object to hold the clock. This requires SUDO access since it is outside the 0xA0000000 to 0xB0000000 region
		clk = axi_init(0xFF5E0000, 0x1000);
	}
	unsigned int reg_value = 0;
	//Obtain the previous clock configuration
	reg_value = axi_read(clk, offset);
	//Clear all of the bits we are configuring as given by the mask
	reg_value = reg_value & (~CLK_MASK);
	//Or the register value with the value we are adding. And it with the mask for robustness though it is not necessary if input arguments are valid
	reg_value = reg_value | (value & CLK_MASK);
	//Write this value to the clock configuration register
	axi_write(clk, offset, reg_value);
	return 0;
}

struct axilite_obj axi_init(unsigned int base_address, unsigned int size)
{
	struct axilite_obj axilite_inst;
	if (base_address >= 0xA0000000 && base_address+size <= 0xB0000000)
	{
		//If entirely within the SUDO free region
		//Use /dev/mpsoc_axireg which doesn't require sudo and which represents memory sector at 0xA000 0000 to 0xB000 0000
		//Since address is offset by 0xA000 0000, subtract this from the desired base address
		if (!non_sudo_fd_initialized)
		{
			//We only need to open the file once, afterwards we save and use the handle
			non_sudo_fd=open("/dev/mpsoc_axiregs",O_RDWR|O_SYNC);
			if (non_sudo_fd==-1)
			{
				//Try again with sudo fd instead
				if (!fd_initialized)
				{
					printf("Non Sudo fd mode failed, trying Sudo\n");
					fd=open("/dev/mem",O_RDWR|O_SYNC);
					if (fd==-1)
					{
						printf("memory map failed!!!");
						assert(false);
						return axilite_inst;
					}
					fd_initialized = true;
				}
				non_sudo_fd = fd;
			}
			else
			{
				non_sudo_fd_used = true;
			}
			non_sudo_fd_initialized=true;
		}
		if (non_sudo_fd_used)
		{
			base_address = base_address - 0xA0000000;
		}
		axilite_inst.map_base=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,non_sudo_fd,base_address);
	}
	else
	{
		//Use /dev/mem which does requre SUDO access and represents the entire memory, starts at address 0x0 so no offset change needed.
		if (!fd_initialized)
		{
			//We only need to open the file once, afterwards we save and use the handle
			fd=open("/dev/mem",O_RDWR|O_SYNC);
			if (fd==-1)
			{
				printf("memory map failed!!!");
				assert(false);
				return axilite_inst;
			}
			fd_initialized=true;
		}
		axilite_inst.map_base=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,base_address);
	}
	axilite_inst.size = size;
	return axilite_inst;
}

unsigned int axi_read(struct axilite_obj axi_obj, unsigned int offset)
{
	//check that the offset is within bounds
	assert (axi_obj.size >= offset);
	//figure out the actual address
	volatile void* virt_addr = (volatile void*)((char*)axi_obj.map_base + offset);
	//extract it from the memory
	unsigned int value = *((unsigned int *) virt_addr);
	//printf ("%u %d\n",value, offset);
	return value;
}

void axi_write(struct axilite_obj axi_obj, unsigned int offset, unsigned int value)
{
	//check that the offset is within bounds
	assert (axi_obj.size >= offset);
	//figure out the actual address
	volatile void* virt_addr = (volatile void*)((char*)axi_obj.map_base + offset);
	//write it to the memory and return
	*((unsigned int *) virt_addr) = value;
}

//Legacy axilite_read and axilite_write are kept for backwards compatibility with previous version

int axilite_read(volatile void* map_base, int offset) 
{
	//figure out the actual address
	volatile void* virt_addr = (volatile void*)((char*)map_base + offset);
	//extract it from the memory
	int value = *((unsigned int *) virt_addr);
	return value;
}

void axilite_write(volatile void* map_base, int offset, int value) 
{
	//figure out the actual address
	volatile void* virt_addr = (volatile void*)((char*)map_base + offset);
	//write it to the memory and return
	*((unsigned int *) virt_addr) = value;
}

void DMA_MM2S(volatile void* map_base, int address_low, int address_high, int size) {
	axilite_write(map_base, DMA_MM2S_OFFSET+DMA_CtrlReg_OFFSET, axilite_read(map_base, DMA_MM2S_OFFSET+DMA_CtrlReg_OFFSET) | XAXIDMA_CR_RUNSTOP_MASK); /* Start the MM2S channel running by setting the run/stop bit to 1*/
	axilite_write(map_base, DMA_MM2S_OFFSET+DMA_MemLoc_OFFSET, address_low);
	axilite_write(map_base, DMA_MM2S_OFFSET+DMA_MemLoc_MSB_OFFSET, address_high);
	axilite_write(map_base, DMA_MM2S_OFFSET+DMA_BUFFLEN_OFFSET, size);
}

void DMA_S2MM(volatile void* map_base, int address_low, int address_high, int size) {
	axilite_write(map_base, DMA_S2MM_OFFSET+DMA_CtrlReg_OFFSET, axilite_read(map_base, DMA_S2MM_OFFSET+DMA_CtrlReg_OFFSET) | XAXIDMA_CR_RUNSTOP_MASK); /* Start the S2MM channel running by setting the run/stop bit to 1*/
	axilite_write(map_base, DMA_S2MM_OFFSET+DMA_MemLoc_OFFSET, address_low);
	axilite_write(map_base, DMA_S2MM_OFFSET+DMA_MemLoc_MSB_OFFSET, address_high);
	axilite_write(map_base, DMA_S2MM_OFFSET+DMA_BUFFLEN_OFFSET, size);
}

void DDR_PS2PL( const array_t *tx_chans,
		axidma_dev_t axidma_dev,
		void *dma_vptr,
		uint64_t start_addr,
		int total_size,
		void *input_buf)
{       
	int size;
	int left_bytes = total_size;
	char * input_buf_dummy = (char *)input_buf;
	do {    
		size = left_bytes > BUFFER_SIZE ? BUFFER_SIZE : left_bytes;
		DMA_S2MM(dma_vptr, low32(start_addr), high32(start_addr), size);
		axidma_oneway_transfer(axidma_dev, tx_chans->data[0], input_buf_dummy, size, true);
		input_buf_dummy += size;
		start_addr += size;
		left_bytes -= size;
	} while (left_bytes != 0);
}

void DDR_PL2PS( const array_t *rx_chans,
		axidma_dev_t axidma_dev,
		void *dma_vptr,
		uint64_t start_addr,
		int total_size,
		void *output_buf)
{       
	int size;
	int left_bytes = total_size;
	char * output_buf_dummy = (char *)output_buf;
	do {    
		size = left_bytes > BUFFER_SIZE ? BUFFER_SIZE : left_bytes;
		DMA_MM2S(dma_vptr, low32(start_addr), high32(start_addr), size);
		axidma_oneway_transfer(axidma_dev, rx_chans->data[0], output_buf_dummy, size, true);
		output_buf_dummy += size;
		start_addr += size;
		left_bytes -= size;
	} while (left_bytes != 0);
}

void timespec_sub(struct timespec *t1, const struct timespec *t2)
{
  assert(t1->tv_nsec >= 0);
  assert(t1->tv_nsec < 1000000000);
  assert(t2->tv_nsec >= 0);
  assert(t2->tv_nsec < 1000000000);
  t1->tv_sec -= t2->tv_sec;
  t1->tv_nsec -= t2->tv_nsec;
  if (t1->tv_nsec >= 1000000000)
  {
    t1->tv_sec++;
    t1->tv_nsec -= 1000000000;
  }
  else if (t1->tv_nsec < 0)
  {
    t1->tv_sec--;
    t1->tv_nsec += 1000000000;
  }
}
