#include<linux/init.h>
#include<linux/module.h>
MODULE_LICENSE("GPL");

static int prog_init(void)
{
	printk(KERN_ALERT "Hey World! What's up?\n \n");
	return 0;
}

static void prog_exit(void)
{
	printk(KERN_ALERT "Leaving this world\n \n");
	
}


module_init(prog_init);
module_exit(prog_exit);
