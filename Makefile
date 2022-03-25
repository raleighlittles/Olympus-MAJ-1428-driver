# Needs to be named the same as your source code file!
obj-m += hid-olympus-maj1428.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean