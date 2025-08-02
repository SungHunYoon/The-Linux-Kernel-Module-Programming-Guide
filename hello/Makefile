obj-m += hello-1.o
obj-m += hello-2.o
obj-m += hello-3.o
obj-m += hello-4.o
obj-m += hello-5.o
obj-m += startstop.o
startstop-objs := start.o stop.o

PWD := $(CURDIR)

all :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) modules

clean :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) clean