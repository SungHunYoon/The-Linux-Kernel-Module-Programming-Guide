obj-m += procfs1.o
obj-m += procfs2.o
obj-m += procfs3.o
obj-m += procfs4.o

PWD := $(CURDIR)

all :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) modules

clean :
	$(MAKE) -C /usr/src/linux-4.20.12-sungyoon M=$(PWD) clean