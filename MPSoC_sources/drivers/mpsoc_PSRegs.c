#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sysfs.h> 
#include <linux/kobject.h> 
#include <asm/io.h>

#define CLK_BASE 0xFF5E0000U
#define PLCLK0 0xC0
#define PLCLK1 0xC4
#define PLCLK2 0xC8
#define PLCLK3 0xCC

#define ENABLE_MASK 0x01000000U
#define ENABLE_MASK_INV 0xFEFFFFFFU
#define ENABLE_AWAYS_TRUE 0x00010002U
#define ENABLE_AWAYS_TRUE_INV 0xFFC0FFFDU
#define FREQ_MASK 0x00003F00U
#define FREQ_MASK_INV 0xFFFFFC0FFU

int base_freq = 800;

volatile int pl0hex;
volatile int pl1hex;
volatile int pl2hex;
volatile int pl3hex;
volatile int pl0freq;
volatile int pl1freq;
volatile int pl2freq;
volatile int pl3freq;
volatile int pl0ena;
volatile int pl1ena;
volatile int pl2ena;
volatile int pl3ena;

static void * pl0clk_virt;
static void * pl1clk_virt;
static void * pl2clk_virt;
static void * pl3clk_virt;

struct kobject *mpsoc_root, *clocks, *fclk0, *fclk1, *fclk2, *fclk3;

int freq_find(int freq) {
	int minIndex = 32;
	int min = 9999;
	int i;
	for (i = 1; i <= 32; i++) {
		if (min > abs(base_freq/i-freq)) {
			min = abs(base_freq/i-freq);
			minIndex = i;
		}
	}
	return minIndex;
}

static ssize_t pl0ena_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", pl0ena);
}

static ssize_t pl0ena_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
	int enatmp;
	if (sscanf(buf,"%d",&enatmp) != 1) {
		printk(KERN_ERR "it is illegal to write non-numeric value to mpsoc regs!\n");
		return count;
	}
	pl0ena = enatmp ? 1 : 0;
	pl0hex = (((pl0hex & ENABLE_MASK_INV) | ((pl0ena << 24) & ENABLE_MASK)) & ENABLE_AWAYS_TRUE_INV) | ENABLE_AWAYS_TRUE;
	if (pl0ena) {
		printk(KERN_INFO "enabling PL CLK0\n");
	} else {
		printk(KERN_INFO "disabling PL CLK0\n");
	}
	writel(pl0hex,pl0clk_virt);
	return count;
}

static ssize_t pl0freq_show(struct device *dev,	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d MHz\n", pl0freq);
}

static ssize_t pl0freq_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
	int freqtmp;
	int divider;
	if (sscanf(buf,"%d",&freqtmp) != 1) {
		printk(KERN_ERR "it is illegal to write non-numeric value to mpsoc regs!\n");
		return count;
	}
	divider = freq_find(freqtmp);
	pl0freq = base_freq / divider;
	printk(KERN_INFO "changing PL CLK0's frequency to %d\n", pl0freq);
	pl0hex = (pl0hex & FREQ_MASK_INV) | ((divider << 8) & FREQ_MASK);
	writel(pl0hex,pl0clk_virt);
	return count;
}

static ssize_t pl1ena_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", pl1ena);
}

static ssize_t pl1ena_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
        int enatmp;
        if (sscanf(buf,"%d",&enatmp) != 1) {
                printk(KERN_ERR "it is illegal to write non-numeric value to mpsoc regs!\n");
                return count;
        }
        pl1ena = enatmp ? 1 : 0;
        pl1hex = (((pl1hex & ENABLE_MASK_INV) | ((pl1ena << 24) & ENABLE_MASK)) & ENABLE_AWAYS_TRUE_INV) | ENABLE_AWAYS_TRUE;
        if (pl1ena) {
                printk(KERN_INFO "enabling PL CLK1\n");
        } else {
                printk(KERN_INFO "disabling PL CLK1\n");
        }
        writel(pl1hex,pl1clk_virt);
        return count;
}

static ssize_t pl1freq_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        return sprintf(buf, "%d MHz\n", pl1freq);
}

static ssize_t pl1freq_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
        int freqtmp;
        int divider;
        if (sscanf(buf,"%d",&freqtmp) != 1) {
                printk(KERN_ERR "it is illegal to write non-numeric value to mpsoc regs!\n");
                return count;
        }
        divider = freq_find(freqtmp);
        pl1freq = base_freq / divider;
        printk(KERN_INFO "changing PL CLK1's frequency to %d\n", pl1freq);
        pl1hex = (pl1hex & FREQ_MASK_INV) | ((divider << 8) & FREQ_MASK);
        writel(pl1hex,pl1clk_virt);
        return count;
}

static ssize_t pl2ena_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", pl2ena);
}

static ssize_t pl2ena_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
        int enatmp;
        if (sscanf(buf,"%d",&enatmp) != 1) {
                printk(KERN_ERR "it is illegal to write non-numeric value to mpsoc regs!\n");
                return count;
        }
        pl2ena = enatmp ? 1 : 0;
        pl2hex = (((pl2hex & ENABLE_MASK_INV) | ((pl2ena << 24) & ENABLE_MASK)) & ENABLE_AWAYS_TRUE_INV) | ENABLE_AWAYS_TRUE;
        if (pl2ena) {
                printk(KERN_INFO "enabling PL CLK2\n");
        } else {
                printk(KERN_INFO "disabling PL CLK2\n");
        }
        writel(pl2hex,pl2clk_virt);
        return count;
}

static ssize_t pl2freq_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        return sprintf(buf, "%d MHz\n", pl2freq);
}

static ssize_t pl2freq_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
        int freqtmp;
        int divider;
        if (sscanf(buf,"%d",&freqtmp) != 1) {
                printk(KERN_ERR "it is illegal to write non-numeric value to mpsoc regs!\n");
                return count;
        }
        divider = freq_find(freqtmp);
        pl2freq = base_freq / divider;
        printk(KERN_INFO "changing PL CLK2's frequency to %d\n", pl2freq);
        pl2hex = (pl2hex & FREQ_MASK_INV) | ((divider << 8) & FREQ_MASK);
        writel(pl2hex,pl2clk_virt);
        return count;
}

static ssize_t pl3ena_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", pl3ena);
}

static ssize_t pl3ena_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
        int enatmp;
        if (sscanf(buf,"%d",&enatmp) != 1) {
                printk(KERN_ERR "it is illegal to write non-numeric value to mpsoc regs!\n");
                return count;
        }
        pl3ena = enatmp ? 1 : 0;
        pl3hex = (((pl3hex & ENABLE_MASK_INV) | ((pl3ena << 24) & ENABLE_MASK)) & ENABLE_AWAYS_TRUE_INV) | ENABLE_AWAYS_TRUE;
        if (pl3ena) {
                printk(KERN_INFO "enabling PL CLK3\n");
        } else {
                printk(KERN_INFO "disabling PL CLK3\n");
        }
        writel(pl3hex,pl3clk_virt);
        return count;
}

static ssize_t pl3freq_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        return sprintf(buf, "%d MHz\n", pl3freq);
}

static ssize_t pl3freq_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
        int freqtmp;
        int divider;
        if (sscanf(buf,"%d",&freqtmp) != 1) {
                printk(KERN_ERR "it is illegal to write non-numeric value to mpsoc regs!\n");
                return count;
        }
        divider = freq_find(freqtmp);
        pl3freq = base_freq / divider;
        printk(KERN_INFO "changing PL CLK3's frequency to %d\n", pl3freq);
        pl3hex = (pl3hex & FREQ_MASK_INV) | ((divider << 8) & FREQ_MASK);
        writel(pl3hex,pl3clk_virt);
        return count;
}

static struct device_attribute pl0clkena = __ATTR(enable, 0664, pl0ena_show, pl0ena_store);
static struct device_attribute pl0clkfreq = __ATTR(frequency, 0664, pl0freq_show, pl0freq_store);
static struct device_attribute pl1clkena = __ATTR(enable, 0664, pl1ena_show, pl1ena_store);
static struct device_attribute pl1clkfreq = __ATTR(frequency, 0664, pl1freq_show, pl1freq_store);
static struct device_attribute pl2clkena = __ATTR(enable, 0664, pl2ena_show, pl2ena_store);
static struct device_attribute pl2clkfreq = __ATTR(frequency, 0664, pl2freq_show, pl2freq_store);
static struct device_attribute pl3clkena = __ATTR(enable, 0664, pl3ena_show, pl3ena_store);
static struct device_attribute pl3clkfreq = __ATTR(frequency, 0664, pl3freq_show, pl3freq_store);

static int __init mpsoc_psregs_init(void) {
	mpsoc_root = kobject_create_and_add("mpsoc",NULL);
	clocks = kobject_create_and_add("clocks",mpsoc_root); 
	fclk0 = kobject_create_and_add("fclk0",clocks);
	fclk1 = kobject_create_and_add("fclk1",clocks);
	fclk2 = kobject_create_and_add("fclk2",clocks);
	fclk3 = kobject_create_and_add("fclk3",clocks);
	if (sysfs_create_file(fclk0,&pl0clkfreq.attr)) return 1;
	if (sysfs_create_file(fclk0,&pl0clkena.attr)) return 1;
	if (sysfs_create_file(fclk1,&pl1clkfreq.attr)) return 1;
	if (sysfs_create_file(fclk1,&pl1clkena.attr)) return 1;
	if (sysfs_create_file(fclk2,&pl2clkfreq.attr)) return 1;
	if (sysfs_create_file(fclk2,&pl2clkena.attr)) return 1;
	if (sysfs_create_file(fclk3,&pl3clkfreq.attr)) return 1;
	if (sysfs_create_file(fclk3,&pl3clkena.attr)) return 1;
	pl0clk_virt = ioremap_nocache(CLK_BASE+PLCLK0,4);
	pl1clk_virt = ioremap_nocache(CLK_BASE+PLCLK1,4);
	pl2clk_virt = ioremap_nocache(CLK_BASE+PLCLK2,4);
	pl3clk_virt = ioremap_nocache(CLK_BASE+PLCLK3,4);
	pl0hex = readl(pl0clk_virt);
	pl1hex = readl(pl1clk_virt);
	pl2hex = readl(pl2clk_virt);
	pl3hex = readl(pl3clk_virt);
	pl0ena = pl0hex >> 24;
	pl1ena = pl1hex >> 24;
	pl2ena = pl2hex >> 24;
	pl3ena = pl3hex >> 24;
	pl0freq = base_freq / ((pl0hex & FREQ_MASK) >> 8);
	pl1freq = base_freq / ((pl1hex & FREQ_MASK) >> 8);
	pl2freq = base_freq / ((pl2hex & FREQ_MASK) >> 8);
	pl3freq = base_freq / ((pl3hex & FREQ_MASK) >> 8);
	printk(KERN_INFO "Finished registering MPSOC sysfs register file group!\n");
	return 0;
}
 
void __exit mpsoc_psregs_exit(void)
{
	iounmap(pl0clk_virt);
	iounmap(pl1clk_virt);
	iounmap(pl2clk_virt);
	iounmap(pl3clk_virt);
	sysfs_remove_file(fclk0, &pl0clkfreq.attr);
	sysfs_remove_file(fclk0, &pl0clkena.attr);
	sysfs_remove_file(fclk1, &pl1clkfreq.attr);
	sysfs_remove_file(fclk1, &pl1clkena.attr);
	sysfs_remove_file(fclk2, &pl2clkfreq.attr);
	sysfs_remove_file(fclk2, &pl2clkena.attr);
	sysfs_remove_file(fclk3, &pl3clkfreq.attr);
	sysfs_remove_file(fclk3, &pl3clkena.attr);
	kobject_put(fclk0);
	kobject_put(fclk1);
	kobject_put(fclk2);
	kobject_put(fclk3);
	kobject_put(clocks);
	kobject_put(mpsoc_root);
	printk(KERN_INFO "Finished unregistering MPSOC sysfs register file group!\n");
}
 
module_init(mpsoc_psregs_init);
module_exit(mpsoc_psregs_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Clark Shen <qianfeng.shen@gmail.com>");
MODULE_DESCRIPTION("MPSOC PS CONFIGURATION REGS driver");
MODULE_VERSION("0");
