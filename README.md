---

# README

---

## Funcionamiento y enfoque lógico

Este proyecto tuvo como meta principal implementar la funcionalidad de `chmod`, que permite modificar los permisos de un archivo. Adicionalmente, creé el archivo `chmodtest.c` para evaluar escenarios comunes relacionados con permisos.

1. **Comando de usuario (`chmod`)**:
   - El usuario puede emplear `chmod <modo> <archivo>` para ajustar los permisos de un archivo específico.
   - Los permisos se representan como un número entero que se almacena en los metadatos del archivo (dentro del `dinode`).

2. **Pruebas con `chmodtest.c`**:
   - Este archivo verifica el comportamiento de `chmod` en varios casos, incluyendo:
     - Creación de archivos.
     - Modificación de permisos.
     - Restricción de cambios en archivos marcados como inmutables.
   - Utiliza llamadas al sistema como `chmod` y `open` para interactuar con el sistema de archivos.

3. **Lógica del kernel**:
   - Cuando se ejecuta el comando `chmod`, se invoca la llamada al sistema (`sys_chmod`), que valida los argumentos, bloquea el inode correspondiente y actualiza los permisos en disco.

4. **Consideraciones de seguridad**:
   - La validación de permisos se realiza en el núcleo para prevenir modificaciones accidentales o malintencionadas. Por ejemplo, algunos archivos están configurados como inmutables y no pueden ser alterados mediante `chmod`.

---

## Detalle de las modificaciones realizadas

### 1. **Estructura del sistema de archivos**
   - **`struct dinode`**: Incorporé un nuevo campo `perm` (int) para almacenar los permisos de cada archivo dentro del `dinode`.
   - **Ajuste de tamaño**: Para evitar problemas de alineación en disco, añadí un campo de relleno (`padding`) que garantiza la consistencia del tamaño del `dinode`.

### 2. **Implementación de la llamada al sistema**
   - En **`sys_chmod`** (ubicado en `kernel/sysfile.c`):
     - Validé los argumentos recibidos (`path` y `mode`).
     - Bloqueé el inode correspondiente usando `ilock`.
     - Modifiqué los permisos accediendo al bloque del `dinode` y actualizando su información.
     - Sincronizé los cambios mediante `log_write` y liberé el inode.
   - Añadí `sys_chmod` en los archivos clave del kernel:
     - `kernel/syscall.c`
     - `kernel/syscall.h`
     - `user/user.h`
     - `user/usys.pl`

### 3. **Comando de usuario**
   - Implementé el comando `chmod` como un programa en `user/chmod.c`.
   - El código valida los argumentos ingresados por el usuario antes de ejecutar la llamada al sistema.

### 4. **Archivo de pruebas: `chmodtest.c`**
   - Diseñé el archivo `chmodtest.c` dentro del directorio `user` para comprobar:
     - La creación correcta de un archivo con permisos predeterminados.
     - Cambios válidos en los permisos.
     - Limitaciones en archivos definidos como inmutables.
   - Este archivo emplea funciones como `open`, `chmod` y `printf` para reportar los resultados.
   - Ejemplo del código:
     ```c
     #include "kernel/types.h"
     #include "kernel/stat.h"
     #include "user/user.h"
     #include "kernel/fcntl.h"

     int main() {
         int fd = open("testfile", O_CREATE | O_RDWR);
         if (fd < 0) {
             printf("Error al crear el archivo.\n");
             exit(1);
         }
         printf("Archivo creado exitosamente.\n");

         if (chmod("testfile", 1) < 0) {
             printf("Error al cambiar los permisos.\n");
             exit(1);
         }
         printf("Permisos modificados correctamente.\n");

         exit(0);
     }
     ```

### 5. **Ajustes en el sistema de compilación**
   - Actualicé el archivo `Makefile` para incluir el nuevo comando `chmod` y el archivo de pruebas `chmodtest.c`.

---

## Desafíos enfrentados y soluciones adoptadas

### 1. **Problemas de alineación en el tamaño del `dinode`**
   - **Inconveniente**: La adición del campo `perm` causó que el tamaño del `dinode` no fuera múltiplo del tamaño de bloque (`BSIZE`), generando errores durante la inicialización.
   - **Solución**: Incluí un campo de relleno (`padding`) para garantizar la alineación adecuada.

### 2. **Errores de compilación en `sys_chmod`**
   - **Inconveniente**: Al principio, el compilador marcaba errores por falta de declaraciones de funciones o estructuras.
   - **Solución**: Verifiqué que todas las declaraciones necesarias estuvieran presentes en los archivos de encabezado correspondientes (`fs.h`, `syscall.h`, etc.) y ajusté las implementaciones.

### 3. **Fallo en el kernel: "log_write outside of trans"**
   - **Inconveniente**: Se intentaba escribir en el log fuera de una transacción activa.
   - **Solución**: Encapsulé las operaciones críticas dentro de una transacción para cumplir con las reglas del sistema de logs.

### 4. **Conflictos al compilar `chmodtest.c`**
   - **Inconveniente**: Existían conflictos entre las definiciones de `fcntl.h` de xv6 y las del sistema host.
   - **Solución**: Usé la versión específica de `fcntl.h` para xv6 y modifiqué el código según fuera necesario.

### 5. **Validación de `chmod`**
   - **Inconveniente**: Era difícil confirmar si los permisos se estaban aplicando correctamente.
   - **Solución**: Añadí múltiples mensajes de depuración (`printf`) para comprobar y ajustar el código hasta que las pruebas pasaran exitosamente.
