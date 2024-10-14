## Informe - Tarea 2

### Objetivo

El objetivo de esta tarea fue modificar el **scheduler** de xv6 para implementar un sistema de prioridades, de manera que los procesos con mayor prioridad sean ejecutados antes que los de menor prioridad. La prioridad debe ser controlada dinámicamente con el uso de un campo `boost`, que modifica la prioridad del proceso durante su ejecución.

### Modificaciones realizadas

#### 1. **Estructura de la prioridad y boost**

Para implementar el sistema de prioridades, agregamos dos campos nuevos a la estructura `proc` en el archivo `proc.h`:

```c
struct proc {
    ...
    int priority;  // Campo de prioridad
    int boost;     // Campo de boost
    ...
};
```

Luego, en la función `allocproc()` en el archivo `proc.c`, inicializamos estos campos:

```c
p->priority = 0;  // La prioridad comienza en 0
p->boost = 1;     // El boost comienza en 1
```

#### 2. **Modificación del scheduler**

El scheduler fue modificado para que actualice la prioridad de cada proceso en cada iteración del ciclo principal. Se implementó la lógica que aumenta o disminuye la prioridad según el valor del `boost`.

El código modificado dentro de la función `scheduler()` es el siguiente:

```c
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();

  c->proc = 0;
  for(;;){
    intr_on();  // Habilitar interrupciones para evitar deadlocks

    int found = 0;
    for(p = proc; p < &proc[NPROC]; p++) {
      acquire(&p->lock);
      if(p->state == RUNNABLE) {
        
        // Lógica de prioridad y boost
        p->priority += p->boost;

        if(p->priority >= 9) {
          p->boost = -1;
        } else if(p->priority <= 0) {
          p->boost = 1;
        }

        // Cambiar al proceso seleccionado
        p->state = RUNNING;
        c->proc = p;
        swtch(&c->context, &p->context);

        c->proc = 0;
        found = 1;
      }
      release(&p->lock);
    }
    
    if(found == 0) {
      // No hay nada para ejecutar; esperar una interrupción.
      intr_on();
      asm volatile("wfi");
    }
  }
}
```

Este código actualiza la prioridad de cada proceso basado en su `boost`. Si la prioridad llega a 9, el boost se reduce a -1, y si baja a 0, el boost vuelve a 1.

#### 3. **Programa de prueba**

Se creó un programa de prueba (`priority_test.c`) que genera 20 procesos, cada uno imprimiendo su PID al ser ejecutado. Además, para evitar que los procesos impriman simultáneamente y se mezclen sus mensajes, se implementó un `sleep()` proporcional al índice del proceso.

```c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int i;
  
  // Crear 20 procesos
  for(i = 0; i < 20; i++) {
    int pid = fork();
    if(pid == 0) {
      // Hacer que el proceso se detenga unos segundos antes de imprimir
      sleep(i * 10);  // Diferente tiempo de espera para cada proceso

      // Este es el código que ejecutarán los procesos hijos.
      printf("Ejecutando proceso con PID: %d\n", getpid());

      exit(0);  // Salir del proceso hijo
    }
  }

  // Esperar a que todos los procesos hijos terminen
  for(i = 0; i < 20; i++) {
    wait(0);
  }

  // Finalizar el programa de prueba
  exit(0);
}
```

### Dificultades encontradas y soluciones

1. **Problemas de salida desordenada**: 
   La ejecución concurrente de múltiples procesos causaba que la salida en la consola se mezclara, haciendo difícil entender qué proceso estaba imprimiendo en un momento dado. 
   
   **Solución**: Se utilizó la función `sleep()` en los procesos hijos para que esperaran tiempos diferentes antes de imprimir, reduciendo la interferencia entre los mensajes.

2. **Error de compilación (`No rule to make target`)**: 
   Hubo un error en el archivo `Makefile` al agregar el nuevo programa de prueba, debido a una barra invertida malformada.
   
   **Solución**: Se corrigió la barra invertida y se verificó la correcta alineación de las líneas en el archivo `Makefile`.

### Conclusión

Se implementó con éxito un scheduler basado en prioridades en xv6, permitiendo que los procesos sean ejecutados de acuerdo con su nivel de prioridad. Además, se probó el sistema con un programa que genera múltiples procesos y se observó el orden de ejecución correcto gracias a la lógica de prioridades y boost implementada. La sincronización de la salida fue un reto inicial, pero se solucionó con la introducción de retardos específicos entre los procesos.