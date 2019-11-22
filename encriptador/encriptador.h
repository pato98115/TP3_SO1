/*
 *  chardev.h - the header file with the ioctl definitions.
 *
 *  Las declaraciones aca dbeen estar en un header file, ya que
 *  deben conocerse tanto en el modulo del kernel(encriptador.c)  
 *  como en el proceso IOCTL (ioctl.c)
 */

#ifndef ENCRIPTADOR_H
#define ENCRIPTADOR_H

#include <linux/ioctl.h>


/* 
 * El numero de dispositivo principal, ya que no podemos 
 * confiar en el registro dinamico, porque ioctl necesita saberlo
 */
#define MAJOR_NUM 200


/* 
 * Configura el mensaje del driver del dispositivo
 */
#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char *)
/*
 * _IOR significa que estamos creando un numero de comando ioctl
 * para pasar informacion de un proceso de usuario al modulo del kernel
 *
 * El primer argumento, MAJOR_NUM, es el numero de dispositivo
 * principal que estamos usando
 *
 * El segundo argumento es el numero de comando
 * (podria haber diferentes significados).
 *
 * El tercer argumento es el tipo que queremos obtener del proceso 
 * al kernel
 */



/* 
 * Obtiene el mensaje del driver del dipositivo
 */
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
/* 
 * Este IOCTL se utiliza para la salida, para obtener el mensaje
 * del driver del dispositivo. Sin embargo, todavia necesitamos el
 * bufer para colocar el mensaje en la entrada, ya que el proceso
 * lo asigna.
 */

/* 
 * Devuelve el numero de bytes del mensaje
 */
#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)
/* 
 * Este IOCTL se usa tanto para entrada como para salida
 * Recibe del usuario un numero n y devuelve un mensaje[n]
 */

/* 
 * El nombre del archivo del dispositivo 
 */
#define DEVICE_FILE_NAME "encryptor_os"

#endif
