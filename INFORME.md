Tarea 1 – Sistemas Operativos

Pasos a seguir para instalar XV6
Primero que nada instalamos Ubuntu en nuestro computador, abrí el PowerShell y coloqué el comando “ wls –install”. Después reinicié el computador para que los cambios hagan efecto. 

En segundo lugar, ya teniendo Ubuntu en nuestro computador, instalé las dependencias necesarias que sería los paquetes con nuestro comando perfecto “sudo apt-get update” y después las herramientas necesarias para que podamos compilar xv6 “sudo apt-get install build-essential qemu-system-x86 git”. Seguidamente clonamos el repositorio de xv6 desde el MIT como se señaló en clases.

Tras clonar el repositorio xv6 usando el mando “git clone https://github.com/mit-pdos/xv6-public.git” y después navegamos en el directorio “cd xv6-public”
Ahora que ya tenemos el clon de nuestro xv6 lo ejecutamos para compilar xv6 con el comando “make” y va a generar los binarios necesarios para ejecutar xv6, también usamos “make qemu” y vamos a explorar ya el emulador de qemu con xv6.

En cuanto a los problemas encontrados fue la dificultad de encontrar un buen video que explique como instalar de forma correcta y sencilla XV6 porque todos los videos que veía eran horribles

imagenes en el zip.