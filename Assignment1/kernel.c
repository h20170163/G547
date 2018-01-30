#include<linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include<linux/random.h>
#include<linux/uaccess.h>
#include<linux/version.h>
  
#include"adc.h"

static dev_t first; // variable for device number
static struct cdev c_dev; // variable for the character device structure
static struct class *cls; // variable for the device class
 
/*****************************************************************************
STEP 4 
my_close(), my_open(), my_read(), my_write() functions are defined here
these functions will be called for close, open, read and write system calls respectively. 
*****************************************************************************/
 
static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "adc0 : open()\n");
    return 0;
}
 
static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "adc0 : close()\n");
    return 0;
}
 
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "adc0 : read()\n");
    return 0;
}
 
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "adc0 : write()\n");
    return len;
}

long my_ioctl(struct file *f, unsigned int ioctl_num, unsigned long ioctl_param)
{
   arg_adc q;
   int adc_val;
   switch (ioctl_num)
   {
      pr_info("Inside IOCTL Function\n");
      case ADC_CHANNEL:
         adc_val=get_random_int()%1024;
      if(copy_from_user(&q,(arg_adc *)ioctl_param,sizeof(arg_adc)))
         return -1;
         switch(q.val_input)
         {
            case 1:
               pr_info("Digital value from ADC1 = %d\n",adc_val);
               break;
            case 2:
               pr_info("Digital value from ADC2 = %d\n",adc_val);
               break;
            case 3:
               pr_info("Digital value from ADC3 = %d\n",adc_val);
               break;
            case 4:
               pr_info("Digital value from ADC4 = %d\n",adc_val);
               break;
            case 5:
               pr_info("Digital value from ADC5 = %d\n",adc_val);
               break;
            case 6:
               pr_info("Digital value from ADC6 = %d\n",adc_val);
               break;
            case 7:
               pr_info("Digital value from ADC7 = %d\n",adc_val);
               break;
            case 8:
               pr_info("Digital value from ADC8 = %d\n",adc_val);
               break;
            default:
               pr_err("Invalid channel ID\n");
               return -1;
         }
      break;
      
      default:
         pr_err("Invalid command code\n");
      return -1;
   }
   q.val_output=adc_val;
   if(copy_to_user((arg_adc *)ioctl_param,&q,sizeof(arg_adc)))
      return -1;

   return 0;
}
//###########################################################################################
 
 
static struct file_operations fops =
{
  .owner    = THIS_MODULE,
  .open     = my_open,
  .release  = my_close,
  .read     = my_read,
  .write    = my_write,
.unlocked_ioctl=my_ioctl
};
  
//########## INITIALIZATION FUNCTION ##################
// STEP 1,2 & 3 are to be executed in this function ### 
static int __init mychar_init(void) 
{
    printk(KERN_INFO "Namaste: adc0 driver registered");
     
    // STEP 1 : reserve <major, minor>
    if (alloc_chrdev_region(&first, 1, 3, "adc_ioctl") < 0)
    {
        return -1;
    }
     
    // STEP 2 : dynamically create device node in /dev directory
    if ((cls = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
        unregister_chrdev_region(first, 1);
        return -1;
    }
    if (device_create(cls, NULL, first, NULL, "adc0") == NULL)
    {
        class_destroy(cls);
        unregister_chrdev_region(first, 1);
        return -1;
    }
     
    // STEP 3 : Link fops and cdev to device node
    cdev_init(&c_dev, &fops);
    if (cdev_add(&c_dev, first, 1) == -1)
    {
        device_destroy(cls, first);
        class_destroy(cls);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    return 0;
}
  
static void __exit mychar_exit(void) 
{
    cdev_del(&c_dev);
    device_destroy(cls, first);
    class_destroy(cls);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Bye: adc0 driver unregistered\n\n");
}
  
module_init(mychar_init);
module_exit(mychar_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aastha Maingi");
MODULE_DESCRIPTION("Character device driver for adc0");
