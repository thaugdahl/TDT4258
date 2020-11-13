#ifndef _KERNEL_GAMEPAD_DRIVER
#define _KERNEL_GAMEPAD_DRIVER
/**
* This is a demo Linux kernel module.
*/
/*
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#include "efm32gg.h"
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/rcupdate.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>


#include "efm32gg.h"


#define GPIO_EVEN_IRQ 17
#define GPIO_ODD_IRQ 18
#define DEVICE_NAME "gamepad"

dev_t dev_no = 1;
struct class *cl;
struct fasync_struct* queue;
struct cdev my_cdev;
static void __iomem *mem_gpio_port_c, *mem_gpio_int;

irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
    printk(KERN_ALERT "Handling GPIO interrupt!\n");
	//printk(KERN_ALERT "GPIO_IF: %x\n", ioread32(GPIO_IF));
	//printk(KERN_ALERT "GPIO_IF (but different): %x\n", ioread32(ioread32(mem_gpio_int + GPIO_IF_OFFSET)));
	//printk(KERN_ALERT "GPIO_IEN: %x\n", ioread32(GPIO_IEN));
	//printk(KERN_ALERT "GPIO_IF: %x\n", ioread32(GPIO_IF));
	
    iowrite32(ioread32(GPIO_IF), mem_gpio_int + GPIO_IFC_OFFSET);	// Clear interrupt flags
	//iowrite32(0xFFFF, mem_gpio_int + GPIO_IFC_OFFSET);	// Clear interrupt flags

    if (queue) {
        kill_fasync(&queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}
static int button_open(struct inode* inode, struct file* filp)
{
    printk(KERN_INFO "Gamepad driver opened\n");
    return 0;
}

static int button_release(struct inode* inode, struct file* filp)
{
    printk(KERN_INFO "Gamepad driver closed\n");
    return 0;
}

static ssize_t button_read(struct file* filp, char* __user buff,
        size_t count, loff_t* offp)
{
    uint32_t button_bitmask = ioread32(GPIO_PC_DIN);
    copy_to_user(buff, &button_bitmask, 1);
    return 1;
}

static int button_fasync(int fd, struct file* filp, int mode) 
{
    return fasync_helper(fd, filp, mode, &queue);
}

struct file_operations fops = {
		.owner = 	THIS_MODULE,
		.open =		button_open,
		.read =		button_read,
		//.write =	button_write,
		.release =	button_release,
		.fasync = 	button_fasync,
	};

/**
* template_init - function to insert this module into kernel space
*
* This is the first of two exported functions to handle inserting this
* code into a running kernel
*
* Returns 0 if successfull, otherwise -1
*/

static int __init template_init(void)
{

	printk("Hello World, here is your module speaking\n");
	
	//Request and map memory (Big cook, much plagiarism)
    //check_mem_region(GPIO_BASE, GPIO_SIZE);
    //request_mem_region(GPIO_BASE, GPIO_SIZE, DEVICE_NAME);

    //makes memory accessable and gets the base value for GPIO operations

	// Q: WHAT the fuck does any of this mean?
	// Tells the kernel that the driver is going to use a range of I/O 
	// addresses. This is purely to reserve the I/O addresses - no memory mapping.
	// This disallows other kernel modules to work on the same addresses.
	/*
	request_mem_region(GPIO_PC_MODEL, 1, DEVICE_NAME);
	request_mem_region(GPIO_PC_DOUT,  1, DEVICE_NAME);
	request_mem_region(GPIO_PA_MODEH, 1, DEVICE_NAME);
	request_mem_region(GPIO_PA_DOUT,  1, DEVICE_NAME);
	request_mem_region(GPIO_PA_CTRL,  1, DEVICE_NAME);
	*/
	printk("You're live on TV, don't say \"Fuck\" or \"Bugger\"\n");
	request_mem_region(GPIO_PC_BASE, GPIO_MODEL_OFFSET, DEVICE_NAME);
	request_mem_region(GPIO_PC_BASE, GPIO_DOUT_OFFSET,  DEVICE_NAME);
	request_mem_region(GPIO_INT_BASE, GPIO_EXTIFALL_OFFSET, DEVICE_NAME);
	request_mem_region(GPIO_INT_BASE, GPIO_IFC_OFFSET, DEVICE_NAME);
	request_mem_region(GPIO_INT_BASE, GPIO_IEN_OFFSET, DEVICE_NAME);

	mem_gpio_port_c = ioremap_nocache(GPIO_PC_BASE, 0x020);
	printk("Fuck! (should be 0x40006048, actual value: 0x%x)\n", mem_gpio_port_c);

	mem_gpio_int = ioremap_nocache(GPIO_INT_BASE, 0x020);
	printk("Bugger! (should be 0x40006100, actual value: 0x%x)\n", mem_gpio_int);

	// Set input pins
	iowrite32(0x33333333, mem_gpio_port_c + GPIO_MODEL_OFFSET);	// Set to input with some settings (Same as every previous exercise. It works)
	iowrite32(0xFF, mem_gpio_port_c + GPIO_DOUT_OFFSET); 		// Pullup? Something like that. Anyway, it's the same as they were before.
	
	// Set interrupts
	iowrite32(0x22222222, mem_gpio_int + GPIO_EXTIPSELL_OFFSET);
	iowrite32(0xFF, mem_gpio_int + GPIO_EXTIFALL_OFFSET);		// Falling edge
	iowrite32(0xFFFF, mem_gpio_int + GPIO_IFC_OFFSET);			// Clear flags
	iowrite32(0x00FF, mem_gpio_int + GPIO_IEN_OFFSET);			// Interrupt enabled

	if(alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME))
	{
		printk("ERROR: Device allocation number failed\n");	
		return -1;

	}else
	{
		printk("Device allocation success\n");
	}
		
	request_irq(GPIO_EVEN_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DEVICE_NAME, &my_cdev);
    request_irq(GPIO_ODD_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DEVICE_NAME, &my_cdev);

	cdev_init(&my_cdev, &fops);
	if(cdev_add(&my_cdev, dev_no, 1))
	{
		printk("ERROR: cdev_add(), whatever the fuck it is, failed :(\n");	
		return -1;

	} else
	{
		printk("cdev_add() success\n");
	}

	cl = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cl, NULL, dev_no, NULL, DEVICE_NAME);

	return 0;

}



/**
* template_cleanup - function to cleanup this module from kernel space
*
* This is the second of two exported functions to handle cleanup this
* code from a running kernel
*/

static void __exit template_exit(void)
{
	printk("You'll never take me alive!!!\n");
	iowrite32(0x0000, GPIO_IEN);

	free_irq(GPIO_EVEN_IRQ, &my_cdev);
	free_irq(GPIO_ODD_IRQ, &my_cdev);

	release_mem_region(GPIO_PC_BASE, GPIO_MODEL_OFFSET);
	release_mem_region(GPIO_PC_BASE, GPIO_DOUT_OFFSET);

	release_mem_region(GPIO_INT_BASE, GPIO_EXTIFALL_OFFSET);
	release_mem_region(GPIO_INT_BASE, GPIO_IFC_OFFSET);
	release_mem_region(GPIO_INT_BASE, GPIO_IEN_OFFSET);

	device_destroy(cl, dev_no);
	class_destroy(cl);
	cdev_del(&my_cdev);

	unregister_chrdev_region(dev_no, 1);

}

module_init(template_init);
module_exit(template_exit);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

#endif