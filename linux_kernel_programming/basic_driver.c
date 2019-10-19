/*
 * This is the example driver explained in driver_basics.txt, but with
 * the very verbose comments removed. 
*/

#include <linux/fs.h> //struct file, struct file_operations
#include <linux/init.h> //for __init, see code
#include <linux/module.h> //for module init and exit macros
#include <linux/miscdevice.h> //for misc_device_register and struct micdev
#include <linux/uaccess.h> //For copy_to_user, see code

static int our_open (struct inode *inode, struct file *filp);
static int our_release (struct inode *inode, struct file *filp);
static ssize_t our_read (struct file *filp, char __user *buf, size_t sz, loff_t *off);

static struct file_operations our_fops = { //Holds pointers to our functions
	.open = our_open,
	.release = our_release, //This is what the close() function calls
	.read = our_read
};


static int our_open (struct inode *inode, struct file *filp) {
	
	if (filp->f_op == &our_fops) {
		printk(KERN_ALERT "Opened our device file! Everything looks okay!\n");
	} else {
		printk(KERN_ERR "Opened our device file, but something looks wrong...\n");
	}
	
	return 0; //SUCCESS
}

static int our_release (struct inode *inode, struct file *filp) {
	printk(KERN_ALERT "Thank you for using our device driver!\n");
	
	return 0;
}

static ssize_t our_read (struct file *filp, char __user *buf, size_t sz, loff_t *off) {
	
	static char msg[] = "Lorem Ipsum\n";
	#define msg_len (sizeof(msg))
	
	int sz_left = (int)msg_len - (int)*off; //Casting to int for safety
	
	//Pick the minimum of sz or sz_left
	int sz_to_send = (int) sz > sz_left ? sz_left : sz;
	
	if (sz_to_send <= 0) {
		//The user has read the entire message. 
		*off = 0;		
		return 0;
	} else {
		copy_to_user(buf, msg + *off, sz_to_send);
		*off += sz_to_send;
		return sz_to_send;
	}
}


static struct miscdevice our_miscdev = { 
	.minor = MISC_DYNAMIC_MINOR, 
	.name = "our_driver",
	.fops = &our_fops,
	.mode = 0666
};

static int registered = 0;



static int __init our_init(void) { 
	
	int err = misc_register(&our_miscdev);
	if (err < 0) {
		printk(KERN_ERR "Could not register misc device\n");
	} else {
		printk(KERN_ALERT "Hello, world!\n"); 
		registered = 1;
	}
	
	return err; //Propagate error code
} 

static void our_exit(void) { 
	if (registered) misc_deregister(&our_miscdev);
	
	printk(KERN_ALERT "Goodbye, cruel world!\n"); 
} 

MODULE_LICENSE("Dual BSD/GPL"); 

module_init(our_init); 
module_exit(our_exit);
