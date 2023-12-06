obj-m=hello.o

KDIR:= /home/lgh/WSL2-Linux-Kernel-linux-msft-wsl-5.15.133.1
PWD:= ${shell pwd}

default:
	make -C $(KDIR) M=$(PWD) modules  
