
obj-m += hello-mod.o
obj-m += second-mod.o
obj-m += firstCharDriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
