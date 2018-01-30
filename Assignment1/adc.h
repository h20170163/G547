#ifndef ADC_H
#define ADC_H

#include <linux/ioctl.h>

typedef struct
{
   int val_input,val_output;
}arg_adc;

#define ADC_CHANNEL _IOR('p',1,arg_adc *)


#endif
