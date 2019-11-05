#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "libaxidma.h"

#define low32(address) ((int)address)
#define high32(address) ((int)(address>>32))

struct axilite_obj
{
	void * map_base;
	unsigned int size;
};

int axilite_read(volatile void* map_base, int offset);
void axilite_write(volatile void* map_base, int offset, int value);
void DMA_S2MM(volatile void* map_base, int address_low, int address_high, int size);
void DMA_MM2S(volatile void* map_base, int address_low, int address_high, int size);
void timespec_sub(struct timespec *t1, const struct timespec *t2);
struct axilite_obj axi_init(unsigned int base_address, unsigned int size);
int pl_clk_config(bool start, int clk_number, int divisor_1, int divisor_2, int type);
unsigned int axi_read(struct axilite_obj axi_obj, unsigned int offset);
void axi_write(struct axilite_obj axi_obj, unsigned int offset, unsigned int value);
void DDR_PS2PL( const array_t *tx_chans,
                axidma_dev_t axidma_dev,
                void *dma_vptr,
                uint64_t start_addr,
                int total_size,
                void *input_buf);

void DDR_PL2PS( const array_t *rx_chans,
                axidma_dev_t axidma_dev,
                void *dma_vptr,
                uint64_t start_addr,
                int total_size,
                void *read_buf);

// CLK constants

#define CLK_MASK 0x13F3F07
#define IOPLL    0
#define RPLL     2
#define DPLL     3


//Other constants

#define BUFFER_SIZE 8*1024*1024-16
#define DMA_BASE        0xA0020000

#define DMA_RegSize	0x00001000

#define DMA_MM2S_OFFSET	0x00000000
#define DMA_S2MM_OFFSET	0x00000030

#define DMA_CtrlReg_OFFSET	0x00000000   /**< Channel control */
#define DMA_StatsReg_OFFSET	0x00000004   /**< Status */

#define DMA_MemLoc_OFFSET	0x00000018
#define DMA_MemLoc_MSB_OFFSET	0x0000001C

#define DMA_BUFFLEN_OFFSET	0x00000028

// @name Bitmasks of DMA_CtrlReg_OFFSET register
#define XAXIDMA_CR_RUNSTOP_MASK 0x00000001 /**< Start/stop DMA channel */
#define XAXIDMA_CR_RESET_MASK   0x00000004 /**< Reset DMA engine */

// @name Bitmasks of DMA_StatsReg_OFFSET register
#define XAXIDMA_HALTED_MASK             0x00000001  /**< DMA channel halted */
#define XAXIDMA_IDLE_MASK               0x00000002  /**< DMA channel idle */
#define XAXIDMA_ERR_SLAVE_MASK          0x00000020  /**< Datamover slave err */
#define XAXIDMA_ERR_DECODE_MASK         0x00000040  /**< Datamover decode*/
#define XAXIDMA_ERR_ALL_MASK            0x00000060  /**< All errors */
