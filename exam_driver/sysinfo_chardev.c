#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/utsname.h>
#include <linux/jiffies.h>
#include <linux/cpumask.h>
#include <linux/string.h>

#define DEVICE_NAME "sysinfo"
#define BUF_LEN 512

// Enumeración para gestionar los modos de visualización
enum info_mode { MODE_ALL, MODE_KERNEL, MODE_CPU, MODE_TIME };
static enum info_mode current_mode = MODE_ALL;

static int major;
static char msg[BUF_LEN];

// Prototipos de funciones del driver
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

// Estructura de operaciones de archivo
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int device_open(struct inode *inode, struct file *file) {
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    module_put(THIS_MODULE);
    return 0;
}

// Función READ: Se ejecuta cuando el usuario hace 'cat /dev/sysinfo'
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;
    
    // Si el offset es mayor a 0, ya terminamos de leer
    if (*offset > 0) return 0;

    // Generar contenido basado en el modo seleccionado por el usuario
    switch (current_mode) {
        case MODE_KERNEL:
            snprintf(msg, BUF_LEN, "Kernel Release: %s\n", utsname()->release);
            break;
        case MODE_CPU:
            snprintf(msg, BUF_LEN, "Number of CPUs: %u\n", num_online_cpus());
            break;
        case MODE_TIME:
            snprintf(msg, BUF_LEN, "System Jiffies: %lu\n", jiffies);
            break;
        case MODE_ALL:
        default:
            snprintf(msg, BUF_LEN, 
                     "--- System Info ---\n"
                     "Kernel Release: %s\n"
                     "Online CPUs   : %u\n"
                     "System Jiffies: %lu\n", 
                     utsname()->release, num_online_cpus(), jiffies);
            break;
    }

    // Copiar el mensaje al espacio de usuario byte a byte de forma segura
    char *ptr = msg;
    while (length && *ptr) {
        if (put_user(*(ptr++), buffer++)) return -EFAULT;
        length--;
        bytes_read++;
    }

    *offset = bytes_read;
    return bytes_read;
}

// Función WRITE: Se ejecuta cuando el usuario hace 'echo "comando" > /dev/sysinfo'
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
    char input[16];
    size_t copy_len = (len < sizeof(input) - 1) ? len : sizeof(input) - 1;

    // Transferencia segura desde el espacio de usuario al kernel
    if (copy_from_user(input, buff, copy_len)) return -EFAULT;
    input[copy_len] = '\0';

    // Lógica de comandos (insensible a mayúsculas/minúsculas)
    if (strncasecmp(input, "all", 3) == 0) {
        current_mode = MODE_ALL;
    } else if (strncasecmp(input, "kernel", 6) == 0) {
        current_mode = MODE_KERNEL;
    } else if (strncasecmp(input, "cpu", 3) == 0) {
        current_mode = MODE_CPU;
    } else if (strncasecmp(input, "time", 4) == 0) {
        current_mode = MODE_TIME;
    } else {
        printk(KERN_WARNING "sysinfo: Invalid command received: %s\n", input);
        return -EINVAL;
    }

    return len;
}

// Inicialización del módulo
int init_module(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "sysinfo: Failed to register major number\n");
        return major;
    }
    printk(KERN_INFO "sysinfo: Registered with major number %d\n", major);
    return 0;
}

// Limpieza del módulo
void cleanup_module(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "sysinfo: Module unregistered\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mateo Onate");
MODULE_DESCRIPTION("Dynamic System Info Character Device for Mid-term Exam");
