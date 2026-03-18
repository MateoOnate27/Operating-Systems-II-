# Nombre del objeto a generar
obj-m += sysinfo_chardev.o

# Detectamos la ruta de las cabeceras del kernel actual
KDIR := /lib/modules/$(shell uname -r)/build

# Forzamos la captura del directorio actual para evitar variables vacias (M=)
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean