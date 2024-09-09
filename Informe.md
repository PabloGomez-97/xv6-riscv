Funcionamiento de las llamadas al sistema
primero utilizamos el getppid(), lo que hace esta llamada es que al sistema devuelve el ID de proceso del padre que la invoca. Es bastante útil para que un proceso conozca quien lo ha creado, lo cual es relevante en tareas de depuración.
En segundo lugar utilizamos getancestor(int n) que lo que hace es devolver el ID del proceso de un ancestro n del proceso que la invoca.

La implementación del getppid()
añadimos la definición del SYS_getppid en el archivo kernel/syscall.h
añadimos la entrada SYS_getppid en el archivo kernel/syscall.c para asociar el número de llamada al sistema con su función correspondiente.
Implementamos la función del nucleo, con sys_getppid() que fue implementada en kernel/sysproc.c para obtener y devolver el ID del proceso padre del proceso actual
modificamos el archivo user.h con la cual añadimos la declaración int getppid(void) para que los programas de usuario puedan invocar la nueva llamada al sistema
Después la modificación del archivo usys.S
Creamos el programa de prueba yosoytupadre.c donde hicimos la prueba mostrando el resultado
Implementamos también el getancestor(int n) 
Los desafios fueron bastantes, en primer lugar tuve varios problemas para correr el xv6 nuevamente, fue algo dificil con lo que tuve que reiniciar todo desde 0, como si estuviera haciendo la tarea 0, pero pude resolverlo y ahora está corriendo todo correctamente. En cuanto a los aprendizajes, fueron también hartos, con ayuda de mi gran amigo chatgpt pudimos resolver los problemas encontrados y también pude aprender a como desarrollarme como informático ante estos problemas que serán cotidianos en el día a día.
