obj-m := kernel.o

all:
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) clean
	clear
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) clean
	
