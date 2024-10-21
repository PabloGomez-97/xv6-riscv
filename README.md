Informe tarea 2

Las modificaciones que se realizaron fueron las siguientes.

1.- En la estructura de la prioridad y boost, implementamos un sistema de prioridades, se agregaron dos campos nuevos en la estructura del "proc" en el archivo de "proc.h". Seguidamente en la función "allocproc" en el archivo "proc.c" se colocaron los siguientes campos en la prorización

p->priority = 0;  // La prioridad comienza en 0

p->boost = 1;     // El boost comienza en 1

2.- Modificamos el scheduler para que se actualizara la prioridad cada vez que el proceso iteraba en el ciclo principal. Se implementó una lógica para aumentar o disminuir la prioridad dependiendo del valor del "boost". Entonces lo que hace es que si la prioridad llega a 9, se va a reducir a -1, y si llega al 0, entonces el boost vuelve a 1.

3.- En el programa de prueba, se creó un programa "priority_test.c" que genera los 20 procesos solicitados, cada uno imprimiendo su PID al ser ejecutado. Para evitar que los procesos imprimiesen simultaneamente los mensajes, se creó un "sleep".

Dificultades encontradas

1. En primer lugar, me encontré con varios problemas pequeños que a medida que iba entendiendo mejor el trabajo las iba resolviendo solo, pero igual las comento porque pueden ser importantes en el aprendizaje.

2. También tuve problemas con la compilación, cuando agregaba nuevos programas al makefile habian varios errores, pero con ayuda de nuestro querido amigo chatgpt y las pude solucionar.

3. Cuando agregué el sleep fue porque estaba teniendo problemas con las salidas desordenadas, se desordenaba todo y hasta yo no entendía lo que hacia, por ende, lo agregué con el fin de que existieran intervalos de tiempo antes de imprimir.
