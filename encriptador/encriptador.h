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
 * El nombre del archivo del dispositivo 
 */
#define DEVICE_FILE_NAME "encryptor_os"

#endif
