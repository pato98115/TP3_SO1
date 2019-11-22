/*
 *  ioctl.c - el proceso para usar ioctl para controlar el modulo del kernel
 *
 * Hasta ahora podriamos haber usado cat para entrada y salida, pero ahora necesitamos hacer ioctl
 * que requiere escribir nuestro propio proceso.
 */

/* 
 * detalles del dispositivo, como numeros ioctl y el archivo principal del dispositivo
 */
#include "encriptador.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>              /* open */
#include <unistd.h>             /* exit */
#include <sys/ioctl.h>          /* ioctl */



/* 
 * Funciones para llamar a ioctl 
 */

ioctl_set_msg(int file_desc, char *message) //Para pasar informacion de un proceso de usuario al modulo del kernel
{
        int ret_val;

        ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);

        if (ret_val < 0) {
                printf("ioctl_set_msg fallo:%d\n", ret_val);
                exit(-1);
        }
}

ioctl_get_msg(int file_desc) //para obtener el mensaje del driver del dispositivo.
{
        int ret_val;
        char message[100];

        /* 
         * Peligroso - esto es peligroso porque no le decimos 
         * al kernel que tan lejos esta permitido escribir, por lo que
         * podria desbordar el bufer. En un programa de produccion real
         * habriamos usado dos ioctls- Uno para decirle al kernel la long
         * del bufer y otro para darle el bufer para llenar.
         */
        ret_val = ioctl(file_desc, IOCTL_GET_MSG, message);

        if (ret_val < 0) {
                printf("ioctl_get_msg fallo:%d\n", ret_val);
                exit(-1);
        }

        printf("get_msg mensaje:%s\n", message);
}

ioctl_get_nth_byte(int file_desc) //Da el numero de bytes del mensaje
{
        int i;
        char c;

        printf("get_nth_byte mensaje:");

        i = 0;
        do {
                c = ioctl(file_desc, IOCTL_GET_NTH_BYTE, i++);

                if (c < 0) {
                        printf
                            ("ioctl_get_nth_byte fallo en el %d'th byte:\n",
                             i);
                        exit(-1);
                }

                putchar(c);
        } while (c != 0);
        putchar('\n');
}

/* 
 * Main - llama las funciones ioctl 
 */
main()
{
        int file_desc, ret_val;
        char *msg = "Mensaje pasado por ioctl\n";

        file_desc = open(DEVICE_FILE_NAME, 0);
        if (file_desc < 0) {
                printf("No se puede abrir el archivo del dispositivo: %s\n", DEVICE_FILE_NAME);
                exit(-1);
        }

        ioctl_get_nth_byte(file_desc);
        ioctl_get_msg(file_desc);
        ioctl_set_msg(file_desc, msg);

        close(file_desc);
}
