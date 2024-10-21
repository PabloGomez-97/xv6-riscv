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
      // Hacer que el proceso se detenga unos segundos
      sleep(i);  // Para que los procesos no impriman todos al mismo tiempo

      // Este es el código que ejecutarán los procesos hijos.
      printf("Ejecutando proceso con PID: %d\n", getpid());

      sleep(10);  // Dormir por 10 ticks después de imprimir

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
