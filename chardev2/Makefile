obj-m += chardev2.o

PWD := $(CURDIR)

all :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) modules
	gcc userspace/userspace_ioctl.c

clean :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) clean
	rm -rf a.out