/**
* This is a demo Linux kernel module.
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
static int __init gamepad_init(void);
static void __exit gamepad_exit(void);


// This is the interrupt handler that will be passed to request_irq as a function pointer
// Upon requesting ownership of the interrupt channels for GPIO_EVEN and GPIO_ODD
// the kernel will be aware of the owner and the interrupt handler to call upon receiving an
// interrupt on these lines.
irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
    iowrite32(ioread32(GPIO_IF), mem_gpio_int + GPIO_IFC_OFFSET);	// Clear interrupt flags
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
		.release =	button_release,
		.fasync = 	button_fasync,
	};

/**
 * Inserts the driver into kernel.
 * Returns 0 on success
*/

static int __init gamepad_init(void)
{
	printk(KERN_INFO "Hello World, here is your module live on TV. Don't say fuck or bugger >:(.\n");
	
	// Allocate memory regions for use by this char device.
	request_mem_region(GPIO_PC_BASE, GPIO_MODEL_OFFSET, DEVICE_NAME);
	request_mem_region(GPIO_PC_BASE, GPIO_DOUT_OFFSET,  DEVICE_NAME);
	request_mem_region(GPIO_INT_BASE, GPIO_EXTIFALL_OFFSET, DEVICE_NAME);
	request_mem_region(GPIO_INT_BASE, GPIO_IFC_OFFSET, DEVICE_NAME);
	request_mem_region(GPIO_INT_BASE, GPIO_IEN_OFFSET, DEVICE_NAME);

	// Get pointers for use with iowrite/ioread for access to the physical address spaces
	mem_gpio_port_c = ioremap_nocache(GPIO_PC_BASE, 0x020);
	if (!mem_gpio_port_c) {
		printk(KERN_WARNING "Fuck! (should be 0x40006048, actual value: 0x%p)\n", mem_gpio_port_c);
	}
	mem_gpio_int = ioremap_nocache(GPIO_INT_BASE, 0x020);
	if (!mem_gpio_int) {
		printk(KERN_WARNING "Bugger! (should be 0x40006100, actual value: 0x%p)\n", mem_gpio_int);
	}
	
	// Setup input pins
	iowrite32(0x33333333, mem_gpio_port_c + GPIO_MODEL_OFFSET);
	iowrite32(0xFF, mem_gpio_port_c + GPIO_DOUT_OFFSET);	
	
	// Setup interrupt registers
	iowrite32(0x22222222, mem_gpio_int + GPIO_EXTIPSELL_OFFSET);
	iowrite32(0xFF, mem_gpio_int + GPIO_EXTIFALL_OFFSET);
	iowrite32(0xFFFF, mem_gpio_int + GPIO_IFC_OFFSET);	
	iowrite32(0x00FF, mem_gpio_int + GPIO_IEN_OFFSET);	

	// Allocate device number for this character device
	// Will return a truthy value on error
	if(alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME))
	{
		printk("ERR: Failed to allocate device number\n");	
		return -1;
	}
		
	// Request ownership of interrupt channels and attach interrupt handler
	// Without requesting ownership, either shared or unshared, the kernel will simply acknowledge and ignore
	// any interrupts coming in on these channels
	request_irq(GPIO_EVEN_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DEVICE_NAME, &my_cdev);
    request_irq(GPIO_ODD_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DEVICE_NAME, &my_cdev);

	// Initialize Character Device
	cdev_init(&my_cdev, &fops);

	// Register the character device, must be done before the kernel invokes this device and its operations
	if(cdev_add(&my_cdev, dev_no, 1))
	{
		printk(KERN_ERR "ERR (%s): Failed to register character device\n", DEVICE_NAME);
		return -1;
	} 
	cl = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cl, NULL, dev_no, NULL, DEVICE_NAME);
	
	printk(KERN_INFO "Gamepad successfully set up\n");
	return 0;
}



/**
* Frees reserved memory, release ownership of interrupt channels, release driver from kernel
*/

static void __exit gamepad_exit(void)
{
	printk("You'll never take me alive!!!\n");
	iowrite32(0x0000, GPIO_IEN);

	iounmap(mem_gpio_port_c);
	iounmap(mem_gpio_int);

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

module_init(gamepad_init);
module_exit(gamepad_exit);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");