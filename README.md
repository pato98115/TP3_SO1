# TP3_SO1
Trabajo practico numero 3 de la materia Sistemas Operativos 1.\
\
*Autores:* Agustinoy Jeremias y Viccini Patricio 

## Descripcion:
>Consiste en dos **módulos** para
insertar en el **kernel** de linux. Estos módulos emulan el funcionamiento de dos dispositivos
de caracteres. Uno de los dispositivos realiza una encriptación simple de los caracteres que
se escriben en el, mientras que el otro de los módulos realiza la desencriptación de los caracteres que se
escriben en el.

---
## **Para el usuario:**


### Cargar un modulo :
>Una vez que el modulo esta *compilado*, para usarlo necesitamos cargarlo al kernel. Esto se hace mediante el comando **sudo insmod <nombre_del_modulo.ko>** .\
>Al ejecutar esto tambien se ejecuta la funcion init del modulo en cuestion. Para nuestro proyecto es importante que luego se proceda a ejecutar el comando **dmesg | tail**, que mostrara la informacion suminstrada por el modulo para asi crear el *device file*.
>Para crear el device file, se debe ejecutar para el encriptador **sudo mknod -m 0666 /dev/encryptor_os c 200 0** y **sudo mknod -m 0666 /dev/decrypter_os c 201 0**, debido a que la informacion es brindada por el paso anterior.
### Encriptador:
>Este modulo es un manejador de dispositivos de caracter que cuando se escribe una
cadena de caracteres en su archivo de dispositivo, toma esa cadena y la “encripta”
*sumando un entero fijo a cada caracter de la cadena*.\
>Cuando se lee del archivo de dispositivo encriptador el controlador entrega la
ultima cadena encriptada.

### Desencriptador :
>Este modulo es un manejador de dispositivos de caracter que cuando se escribe una
cadena de caracteres en su archivo de dispositivo, toma esa cadena y la “desencripta”
*restando el mismo entero del primer manejador a cada caracter de la cadena*.\
>Cuando se lee del archivo de dispositivo desencriptador el controlador entrega la
ultima cadena desencriptada.

### Probar los modulos :
> * **Encriptar una cadena:** se puede escribir en el device file del encriptador mediante, por ejemplo, el comando **echo *cadena* > /dev/encryptor_os**.
> * **Leer cadena encriptada:** se puede leer del device file del encriptador mediante, por ejemplo, el comando **cat /dev/encryptor_os**.
>* **Desencriptar una cadena:** se puede escribir en el device file del desencriptador mediante, por ejemplo, el comando **echo *cadena* > /dev/decrypter_os**.
> * **Leer cadena desencriptada:** se puede leer del device file del encriptador mediante, por ejemplo, el comando **cat /dev/decrypter_os**.

### Para eliminar los modulos y archivos de modulo :
> Para eliminar el modulo del encriptador se ejecuta el comando: **sudo rmmod encriptador** 
> Para eliminar el archivo del modulo encriptador se ejecuta el comando: **sudo rm /dev/encryptor_os**
> Para eliminar el modulo del desencriptador se ejecuta el comando: **sudo rmmod desencriptador** 
> Para eliminar el archivo del modulo desencriptador se ejecuta el comando : **sudo rm /dev/decrypter_os**
---
## **Para el desarrollador:**

