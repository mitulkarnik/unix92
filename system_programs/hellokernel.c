#include <linux/kernel.h>	// for kernel functions 
#include <linux/module.h>	// for working with modules 
#include <linux/init.h>		// for using init macros 
#include <linux/slab.h>

MODULE_AUTHOR("Mitul Karnik");
MODULE_DESCRIPTION("Simple hellow wordl program for Git");
MODULE_LICENSE("GPL");

static int __init init_routine(void)
{
	char *message = (char *)kmalloc(sizeof(char)*40,GFP_KERNEL);
	strcpy(message,"Greetings from Kernel world");
	
	printk(KERN_INFO "Kernel: %s \n",message);
	kfree(message);
	return 0;
}

static void __exit exit_routine(void)
{
	printk(KERN_INFO "Kernel: Goodbye\n");
}

module_init(init_routine);
module_exit(exit_routine);

