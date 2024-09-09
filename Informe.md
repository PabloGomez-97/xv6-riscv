Informe de Implementación de Llamadas al Sistema en xv6
Introducción
En esta implementación, desarrollamos dos nuevas llamadas al sistema en xv6: getppid() y getancestor(int n). Cada una de ellas aporta funcionalidad clave para la interacción entre procesos y su jerarquía, siendo útiles tanto para la depuración como para el seguimiento de la genealogía de procesos.

Funcionalidad de las Llamadas al Sistema
getppid():
Esta llamada devuelve el ID del proceso padre del proceso que la invoca. Es fundamental en tareas de depuración, ya que permite a un proceso conocer quién lo ha creado. Esta información es valiosa para entender la estructura de los procesos y su relación en el sistema operativo.

getancestor(int n):
Esta llamada extiende la funcionalidad de getppid() al devolver el ID de un ancestro n-ésimo del proceso que la invoca. De esta forma, un proceso puede rastrear no solo a su padre inmediato, sino a cualquier antepasado en la jerarquía de procesos.

Implementación
getppid()
Definición de la llamada:

Se añadió la definición de SYS_getppid en el archivo kernel/syscall.h.
Se agregó la entrada SYS_getppid en kernel/syscall.c, asociando el número de llamada al sistema con la función correspondiente.
Función de núcleo:

Se implementó la función del núcleo sys_getppid() en kernel/sysproc.c, encargada de obtener y devolver el ID del proceso padre del proceso actual.
Modificación de archivos de usuario:

Se modificó el archivo user.h añadiendo la declaración int getppid(void) para permitir que los programas de usuario invoquen esta nueva llamada al sistema.
En usys.S, se añadieron las instrucciones necesarias para soportar la nueva llamada.
Prueba:

Se creó el programa de prueba yosoytupadre.c, que ejecuta la llamada getppid() y muestra el resultado correctamente.
getancestor(int n)
La implementación de esta llamada siguió un procedimiento similar, ajustando la lógica para devolver el ancestro solicitado.
Desafíos y Aprendizajes
El proceso de implementación no estuvo exento de dificultades. Uno de los desafíos principales fue restablecer el entorno de xv6 después de varios errores críticos. Esto implicó reiniciar el sistema desde cero, como si fuera la primera tarea. Sin embargo, superar estos obstáculos me permitió no solo ejecutar correctamente el sistema sino también profundizar en el funcionamiento interno de xv6.

En cuanto a los aprendizajes, la experiencia fue sumamente enriquecedora. Con la asistencia de mi invaluable compañero, ChatGPT, pude resolver varios problemas y desarrollar habilidades clave para abordar los retos diarios como ingeniero informático.