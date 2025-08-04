obj-m += completions.o

PWD := $(CURDIR)

all :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) modules

clean :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) clean