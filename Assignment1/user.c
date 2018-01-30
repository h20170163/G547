#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/ioctl.h>
#include "adc.h"


int main()
{
   arg_adc q;
   int file,value;
   char *device="/dev/adc0";
   file=open(device,0);

   if(file<0)
   {
      printf("File did not open from user space\n");
      return -1;
   }
   printf("Enter ADC channel ID\n");
   scanf("%d",&value);
   q.val_input=value;
   if(ioctl(file,ADC_CHANNEL,&q)<0)
   {
      printf("IOCTL failed in kernel space \n");
      return -1;
   }
   printf("ADC value from Device%d is = %d\n",value,q.val_output);
   return 0;
}
