#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <asm/io.h>
#include <linux/delay.h>


typedef struct 
{
   int val1,val2;
}arg_timer0;



static dev_t timer0; // Global variable for the timer0 device number
static struct cdev c_dev; // variable for the character device structure
static struct class *cls; // variable for the device class

#define data_8254	0x40
#define command_8254	0x43


int timer_initialize(void)
{
printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);

outb_p(0x30, command_8254);
outb_p(0xA9, data_8254);
outb_p(0x04, data_8254);
return 0;
}

int timer_sleep(void)
{
printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);
printk(KERN_ALERT "Timer going to sleep\n");


outb_p(0x30, command_8254);
outb_p(0xAA, data_8254);
outb_p(0xFF, data_8254);

printk(KERN_ALERT "Timer woke up from sleep\n");

return 0;
}


int simple_open(struct inode *pinode, struct file *file)
{
printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);
return 0;
}

ssize_t simple_read(struct file *file, char __user *user, size_t length, loff_t *offset)
{
printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);
return 0;
}

ssize_t simple_write(struct file *file, const char __user *user, size_t length, loff_t *offset)
{
printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);
return length;
}

int simple_close(struct inode *pinode, struct file *file)
{
printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);
return 0;
}

long my_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)

{
printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);

arg_timer0 t;

t.val1 = inb_p(data_8254);
t.val2 = inb_p(data_8254);



copy_to_user((arg_timer0 *)ioctl_param,&t,sizeof(arg_timer0));
return 0;
}


//To hold the file operations performed on this device
struct file_operations simple_file_op={
.owner=THIS_MODULE,
.open=simple_open,
.read=simple_read,
.write=simple_write,
.release=simple_close,
.unlocked_ioctl=my_ioctl
};


int __init init_module(void)
{

printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);
 printk(KERN_INFO "Timer0 driver registered\n\n");
    if (alloc_chrdev_region(&timer0, 0, 1, "Timer0 character device driver") < 0)
    {
        return -1;
    }
    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(timer0), MINOR(timer0));

if (alloc_chrdev_region(&timer0, 0, 1, "Timer0 character device driver") < 0)
    {
        return -1;
    }
     
    // STEP 2 : dynamically create device node in /dev directory
    if ((cls = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
        unregister_chrdev_region(timer0, 1);
        return -1;
    }
    if (device_create(cls, NULL, timer0, NULL, "timer0") == NULL)
    {
        class_destroy(cls);
        unregister_chrdev_region(timer0, 1);
        return -1;
    }
     
    // STEP 3 : Link fops and cdev to device node
    cdev_init(&c_dev, &simple_file_op);
    if (cdev_add(&c_dev, timer0, 1) == -1)
    {
        device_destroy(cls, timer0);
        class_destroy(cls);
        unregister_chrdev_region(timer0, 1);
        return -1;
    }

timer_initialize();

struct data {
int status1, status2;
};

struct data mystruct;

mystruct.status1 = inb_p(data_8254);
mystruct.status2 = inb_p(data_8254);


printk(KERN_ALERT "The current count value of the timer0 is %d%d \n", mystruct.status1, mystruct.status2);

timer_sleep();


return 0;
}

void __exit cleanup_module()
{
printk(KERN_ALERT "Inside the %s function \n", __FUNCTION__);
 unregister_chrdev_region(timer0, 1);
    cdev_del(&c_dev);
    device_destroy(cls, timer0);
    class_destroy(cls);
printk(KERN_INFO "Timer0 driver unregistered");
    
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aastha Maingi");
MODULE_DESCRIPTION("Timer0 Driver");

