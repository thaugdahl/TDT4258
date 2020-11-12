#ifndef _KERNEL_GAMEPAD_DRIVER
#define _KERNEL_GAMEPAD_DRIVER
/*
* This is a demo Linux kernel module.
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

dev_t dev_no = 1;
struct class *cl;

struct file_operations fops = {
		.owner = 	THIS_MODULE,
		.read =		read,
		.write =	write,
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
		
	struct cdev *my_cdev = cdev_alloc();
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

	// ??!!!???!??! WHAT the fuck does any of this mean?
	request_mem_region(GPIO_PC_MODEL, 1, "gamepad");
	request_mem_region(GPIO_PC_DOUT,  1, "gamepad");
	request_mem_region(GPIO_PA_MODEH, 1, "gamepad");
	request_mem_region(GPIO_PA_DOUT,  1, "gamepad");
	request_mem_region(GPIO_PA_CTRL,  1, "gamepad");

	iowrite32(2, GPIO_PA_CTRL);
	iowrite32(0x55555555, GPIO_PA_MODEH);
	iowrite32(0xFFFF, GPIO_PA_DOUT); 
	iowrite32(0x33333333, GPIO_PC_MODEL); 
	iowrite32(0xFF, GPIO_PC_DOUT); 


	cl = class_create(THIS_MODULE, NAME);
    device_create(cl, NULL, dev_no, NULL, NAME);

	return 0;

}

/**
* function to set up GPIO modes
* ! COPIED FROM EXERCISE 2, NOT CHANGED! NO GOOD? NO GOOD!!
*/ 
/*
void setupGPIO() 
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	// enable GPIO clock 
	
	// Set up output pins
	iowrite32(2, GPIO_PA_CTRL) 
	*GPIO_PA_CTRL = 2;							// set high drive strength 
	*GPIO_PA_MODEH = 0x55555555;				// set pins A8-15 as output 
	*GPIO_PA_DOUT = 0xFFFF;						// Ensure all LED-light starts turned off 


	// Set up input pins
	*GPIO_PC_MODEL = 0x33333333;
	*GPIO_PC_DOUT = 0xFF;
} 	
*/

/*
* template_cleanup - function to cleanup this module from kernel space
*
* This is the second of two exported functions to handle cleanup this
* code from a running kernel
*/

static void __exit template_cleanup(void)
{
	 printk("You'll never take me alive!!!\n");
	 unregister_chrdev_region(dev_no, 1);
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

#endif