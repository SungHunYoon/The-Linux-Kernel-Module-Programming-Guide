obj-m += sleep.o

PWD := $(CURDIR)

all :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) modules
	gcc cat_nonblock.c

clean :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) clean
	rm -rf a.out