#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/ioctl.h>



typedef struct
{
   int val1,val2;
}arg_timer0;

#define timer0 _IOR('p', 1, arg_timer0 *)

int main()
{
   arg_timer0 t;
   int file,value;
   char *device="/dev/timer0";
   file=open(device,0);

   if(file<0)
   {
      printf("File did not open from user space\n");
      return -1;
   }

   if(ioctl(file,timer0,&t)<0)
   {
      printf("IOCTL failed in kernel space \n");
      return -1;
   }
   printf("The current value of the timer0 is = %d%d\n",t.val1,t.val2);
   return 0;
}
