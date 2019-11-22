/*
 *  encryptor.c - Create an input/output character device
 */

#include <linux/kernel.h>       /* We're doing kernel work */
#include <linux/module.h>       /* Specifically, a module */
#include <linux/fs.h>
#include <asm/uaccess.h>        /* for get_user and put_user */

#include "encriptador.h"
#define SUCCESS 0
#define DEVICE_NAME "os_charencryptor"
#define BUF_LEN 80

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Agustinoy Jeremias");
MODULE_DESCRIPTION("Encriptador de datos");
MODULE_VERSION("0.1");

/* 
 * Esta bandera se usa para prevenir el acceso concurrente al dispositivo
 */
static int Device_Open = 0;

/* 
 * El mensaje que darael dispositivo cuando se le solicite
 */
static char Message[BUF_LEN];

/* 
 * Hasta donde llego el proceso de lectura del mensaje?
 * Util si el mensaje es mas grande que el tamaño del bufer 
 * Nosotros podemos llenar device_read 
 */
static char *Message_Ptr;

/* 
 * Esto se llama cada vez que un proceso intenta abrir el archivo del dispositivo
 */
static int device_open(struct inode *inode, struct file *file)
{
#ifdef DEBUG
        printk(KERN_INFO "device_open(%p)\n", file);
#endif

        /* 
         * No queremos hablar con dos procesos al mismo tiempo
         */
        if (Device_Open)
                return -EBUSY;

        Device_Open++;
        /*
         * Inicializa el mensaje 
         */
        Message_Ptr = Message;
        try_module_get(THIS_MODULE);
        return SUCCESS;
}

/* 
 * Esto se llama cada vez que un proceso intenta cerrar el archivo del dispositivo
 */
static int device_release(struct inode *inode, struct file *file)
{
#ifdef DEBUG
        printk(KERN_INFO "device_release(inodo:%p,file:%p)\n", inode, file);
#endif

        /* 
         * Estamos preparados para la proxima llamada
         */
        Device_Open--;

        module_put(THIS_MODULE);
        return SUCCESS;
}

/* 
 * Esta funcion se llama cada vez que un proceso ya ha abierto el archivo
 * del dispositivo e intenta leerlo.
 */
static ssize_t device_read(struct file *file,           /* see include/linux/fs.h   */
                           char __user * buffer,        /* bufer para ser llenado con datos     */
                           size_t length,               /* longitud del buffer  */
                           loff_t * offset)
{
        /* 
         * Numero de bytes actualmente leidos por el bufer
         */
        int bytes_read = 0;

#ifdef DEBUG
        printk(KERN_INFO "device_read(file:%p,buffer:%p,longitud:%d)\n", file, buffer, length);
#endif

        /* 
         * Cuando estamos al final del archivo, retornamos 0
         * Que significa el final del archivo.
         */
        if (*Message_Ptr == 0)
                return 0;

        /* 
         * Pone los datos en el bufer
         */
        while (length && *Message_Ptr) {

                /* 
                 * Debido a que el bufer esta en el segmento de datos del usuario,
                 * no en el segmento de datos del kernel, la asignacion no funcionaria
                 * En cambio. tenemos que usar put_user que copia los datos del kernel
                 * al segmento de datos del usuario
                 */
                put_user(*(Message_Ptr++), buffer++);
                length--;
                bytes_read++;
        }

#ifdef DEBUG
        printk(KERN_INFO "Read %d bytes, %d left\n", bytes_read, length);
#endif

        /* 
         * Se supone que las funciones de lectura devuelven el numero
         * de bytes realmente insertados en el bufer 
         */
        return bytes_read;
}

/* 
 * Esta funcion se llama cuando alguien intenta escribir en el archivo
 * de nuestro dispositivo
 */
static ssize_t device_write(struct file *file,
                            const char __user * buffer,
                            size_t length, loff_t * offset)
{
        int i;

#ifdef DEBUG
        printk(KERN_INFO "device_write(file:%p,buffer:%s,longitud:%d)", file, buffer, length);
#endif

		
        for (i = 0; i < length && i < BUF_LEN; i++)
                get_user(Message[i], buffer + i);
        
        //Este for es el que toma los caracteres ingresados por el usuario y les suma 10 para encriptarlos.
        for (i = 0; i < length && i < BUF_LEN; i++)
				Message[i] = Message[i]+10;

        Message_Ptr = Message;

        /* 
         * Devuelve el numero de caracteres de entrada usados. 
         */
        return i;
}

/* 
 * Esta funcion se llama cada vez que un proceso intenta hacer un ioctl sobre nuestro archivo de dispositivo. 
 * Tenemos dos parametros extras (adicional a las estructuras de inodo y archivo
 * que obtienen todas las funciones del dispotivivo): el numero de ioctl es llamado
 * y el numero de parametros dado por la funcion ioctl.
 *
 * Si el ioctl es de escritura o escritura/lectura (lo que significa que la salida se 
 * devuelve al proceso de llamada), la llamada ioctl retorna la salida de esta funcion
 *
 */
long device_ioctl(struct file *file,     /* ditto */
                 unsigned int ioctl_num,        /* numero y parametro para IOCTL */
                 unsigned long ioctl_param)
{
        int i;
        char *temp;
        char ch;

        /* 
         * Switch de acuerdo al IOCTL llamado 
         */
        switch (ioctl_num) {
            case IOCTL_SET_MSG:
                    /* 
                     * Recibe un puntero para un mensaje (en espacio de usuario) y configura
                     * para que sea el mensaje del dispositivo. Obtiene el parametro dado 
                     * por el IOCTL del proceso
                     */
                    temp = (char *)ioctl_param;

                    /* 
                     * Encuentra la longitud del mensaje
                     */
                    get_user(ch, temp);
                    for (i = 0; ch && i < BUF_LEN; i++, temp++)
                            get_user(ch, temp);

                    device_write(file, (char *)ioctl_param, i, 0);
                    break;

            case IOCTL_GET_MSG:
                    /* 
                     * Dar el mensaje actual al proceso de llamada,
                     * el parametro que obtuvimos es un puntero, llenarlo.
                     */
                    i = device_read(file, (char *)ioctl_param, 99, 0);

                    /* 
                     * Pone un cero al final del bufer, para que se termine
                     * correctamente
                     */
                    put_user('\0', (char *)ioctl_param + i);
                    break;

            case IOCTL_GET_NTH_BYTE:
                    /* 
                     * Este ioctl esta entre la entrada (ioctl_param) y 
                     * la salida (el retorno del valor de esta funcion) 
                     */
                    return Message[ioctl_param];
                    break;
        }

        return SUCCESS;
}

/* Module Declarations */

/* 
 * Esta estructura contendra funciones que se llamaran
 * cuando un proceso haga algo al dispositivo que creamos
 * Dado que un puntero a esta estructura se mantiene en la tabla
 * de dispositivos, no puede ser local para init_module. 
 * NULL es para funcion no implementada
 */
struct file_operations Fops = {
        .read = device_read,
        .write = device_write,
        .unlocked_ioctl = device_ioctl,
        .open = device_open,
        .release = device_release,      /* a.k.a. close */
};

/* 
 * Initialize the module - Register the character device 
 */
int init_module()
{
        int ret_val;
        /* 
         * Registrar el dispositivo de caracteres (al menos intentarlo) 
         */
        ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

        /* 
         * Valores negativos significan un error 
         */
        if (ret_val < 0) {
                printk(KERN_ALERT "%s failed with %d\n",
                       "Sorry, registering the character device ", ret_val);
                return ret_val;
        }

        printk(KERN_INFO "%s The major device number is %d.\n",
               "Registeration is a success", MAJOR_NUM);
        printk(KERN_INFO "If you want to talk to the device driver,\n");
        printk(KERN_INFO "you'll have to create a device file. \n");
        printk(KERN_INFO "We suggest you use:\n");
        printk(KERN_INFO "mknod %s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM);
        printk(KERN_INFO "The device file name is important, because\n");
        printk(KERN_INFO "the ioctl program assumes that's the\n");
        printk(KERN_INFO "file you'll use.\n");

        return 0;
}

/* 
 * Cleanup - unregister the appropriate file from /proc 
 */
void cleanup_module()
{
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);

	/* 
	 * If there's an error, report it 
	 */
	
}

