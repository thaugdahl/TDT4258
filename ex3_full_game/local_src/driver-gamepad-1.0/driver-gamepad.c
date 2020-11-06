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

/*
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
	/*struct file_operations fops = {
		owner THIS_MODULE,
		read read,
		write write
	}*/
	printk("Hello World, here is your module speaking\n");
	//int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);	
	if(alloc_chrdev_region(dev_no, 0, 1, *name))
	{
		printk("ERROR: Device allocation number failed\n");	
	}else
	{
		printk("Device allocation success/n");
	}
	
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
	 // unregister_chrdev_region(dev_no, 1);
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

#endif