#ifndef _KERNEL_GAMEPAD_DRIVER
#define _KERNEL_GAMEPAD_DRIVER
/*
* This is a demo Linux kernel module.
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#define GPIO_EVEN_IRQ 17
#define GPIO_ODD_IRQ 18


dev_t dev_no = 1;
struct class *cl;
struct fasync_struct* queue;
struct cdev *my_cdev;


struct file_operations fops = {
		.owner = 	THIS_MODULE,
		.open =		buttons_open,
		.read =		buttons_read,
		//.write =	buttons_write,
		.release =	buttons_release,
		.fasync = 	buttons_fasync,
	}


irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
    printk(KERN_ALERT "Handling GPIO interrupt\n");
    iowrite32(ioread32(GPIO_IF), GPIO_IFC);	// Clear interrupt flags
    if (queue) {
        kill_fasync(queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}

static int buttons_fasync(int fd, struct file* filp, int mode) 
{
    return fasync_helper(fd, filp, mode, &queue);
}

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
	char name[] = "gamepad";
	

	printk("Hello World, here is your module speaking\n");
	//int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);	
	if(alloc_chrdev_region(dev_no, 0, 1, *name))
	{
		printk("ERROR: Device allocation number failed\n");	
		return -1;

	}else
	{
		printk("Device allocation success\n");
	}
		
	request_irq(GPIO_EVEN_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &my_cdev);
    request_irq(GPIO_ODD_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &my_cdev);

	*my_cdev = cdev_alloc();
	my_cdev->ops = &fops;
	cdev_init(*my_cdev, *fops);
	if(cdev_add(*my_cdev, dev_no, 1))
	{
		printk("ERROR: cdev_add(), whatever the fuck it is, failed :(\n");	
		return -1;

	}else
	{
		printk("cdev_add() success\n");
	}

	cl = class_create(THIS_MODULE, NAME);
    device_create(cl, NULL, dev_no, NULL, NAME);

	// ??!!!???!??! WHAT the fuck does any of this mean?
	request_mem_region(GPIO_PC_MODEL, 1, name);
	request_mem_region(GPIO_PC_DOUT,  1, name);
	request_mem_region(GPIO_PA_MODEH, 1, name);
	request_mem_region(GPIO_PA_DOUT,  1, name);
	request_mem_region(GPIO_PA_CTRL,  1, name);

	/* Set up output pins */
	iowrite32(2, GPIO_PA_CTRL);				// High drive strength
	iowrite32(0x55555555, GPIO_PA_MODEH);	// A8-A15 as output
	iowrite32(0xFFFF, GPIO_PA_DOUT); 		// Set to high (LEDs are off) by default
	iowrite32(0x33333333, GPIO_PC_MODEL); 	// Set to input with some settings (Same as every previous exercise. It works)
	iowrite32(0xFF, GPIO_PC_DOUT);			// Pullup? Something like that. Anyway, it's the same as they were before.
	
	 
	iowrite32(0xFF, GPIO_EXTIFALL);
    iowrite32(0x00FF, GPIO_IEN);
    iowrite32(0xFF, GPIO_IFC);

	return 0;

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


/*
* template_cleanup - function to cleanup this module from kernel space
*
* This is the second of two exported functions to handle cleanup this
* code from a running kernel
*/

static void __exit gamepad_cleanup(void)
{
	printk("You'll never take me alive!!!\n");
	iowrite32(0x0000, GPIO_IEN);

	free_irq(GPIO_EVEN_IRQ, &my_cdev);
	free_irq(GPIO_ODD_IRQ, &my_cdev);

	release_mem_region(GPIO_PC_MODEL, 1);
	release_mem_region(GPIO_PC_DOUT,  1);
	release_mem_region(GPIO_PA_MODEH, 1);
	release_mem_region(GPIO_PA_DOUT,  1);
	release_mem_region(GPIO_PA_CTRL,  1);

	device_destroy(cl, dev_no);
	class_destroy(cl);
	cdev_del(&my_cdev);

	unregister_chrdev_region(dev_no, 1);

}

module_init(template_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

#endif