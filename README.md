**README**

### Implementación de Protección de Memoria en xv6

Este documento detalla cómo se implementó la protección de memoria en xv6 mediante las llamadas al sistema `mprotect` y `munprotect`, que permiten marcar regiones de memoria como solo lectura o restablecerlas a lectura/escritura.

---

### Lógica de la Protección de Memoria

La protección de memoria permite a una aplicación restringir el acceso a partes específicas de su memoria, evitando la modificación de datos en áreas sensibles. La lógica básica de esta implementación es:

-   **mprotect**: Establece una región de memoria como de solo lectura. Modificando el bit de escritura (PTE_W) en la entrada de la tabla de páginas, esta syscall previene la modificación de esa región.
-   **munprotect**: Revierte la protección de solo lectura, restaurando los permisos de escritura en la región previamente protegida. Esto se logra activando el bit de escritura (PTE_W) en las entradas correspondientes.

Ambas funciones operan sobre direcciones alineadas a páginas, procesando las páginas dentro de la longitud especificada. En caso de que la dirección no sea válida o que una página no esté presente, ambas funciones retornan un error.

---

### Detalles de los Cambios Realizados

Para implementar `mprotect` y `munprotect` en xv6, se realizaron los siguientes ajustes:

#### Definición de Syscalls en syscall.h

-   Se añadieron `SYS_mprotect` y `SYS_munprotect` con identificadores únicos para habilitar estas nuevas syscalls.

#### Funciones Envolventes en sysproc.c

-   Se implementaron `sys_mprotect` y `sys_munprotect`, que obtienen los argumentos desde el espacio de usuario y llaman a las funciones correspondientes `mprotect` y `munprotect`.
-   Se utilizó `fetchaddr` para obtener la dirección base, y una nueva función `fetchint` para extraer la longitud desde el espacio de usuario.

#### Implementación de `mprotect` y `munprotect` en proc.c

-   **mprotect**: Recorre cada página en la región especificada, encuentra la entrada en la tabla de páginas con `pte_lookup` (una función creada para obtener la dirección de la entrada de la tabla de páginas) y desactiva el bit de escritura.
-   **munprotect**: De manera similar, pero restablece el bit de escritura en cada entrada de página en la región especificada.

La función `pte_lookup` fue desarrollada para obtener el puntero a `pte_t` para una dirección virtual específica, funcionando de manera similar a `uvmlookup`.

#### Función `fetchint` en syscall.c

-   `fetchint` se creó para extraer argumentos de tipo int directamente desde el espacio de usuario, usando `fetchaddr` como referencia.

---

### Pruebas

Las funciones fueron probadas mediante el archivo `test_mprotect.c`.

---

### Desafíos y Soluciones Implementadas

-   **Error con `argaddr` y `argint`**: Inicialmente, se intentó usar `argaddr` y `argint` para extraer los argumentos, pero surgieron errores de tipo `void value not ignored as it ought to be`. La solución fue implementar `fetchint`, que fue diseñada tomando `fetchaddr` como base.

-   **Ausencia de `uvmlookup` en xv6 RISC-V**: Dado que xv6 RISC-V no tiene la función `uvmlookup`, se implementó `pte_lookup`, que navega por las tablas de páginas para localizar la entrada `pte_t` para una dirección virtual específica.

-   **Errores de Compilación en la Tabla de Syscalls**: En la tabla `syscalls[]` en `syscall.c`, hubo problemas de tipos al enlazar `sys_mprotect` y `sys_munprotect`. Se resolvió definiendo estas funciones sin parámetros y usando `fetchaddr` y `fetchint` para extraer los argumentos.

-   **Verificación de Páginas Válidas y Presentes**: Para evitar errores en páginas inválidas o ausentes, se añadió una verificación en `pte_lookup` que se asegura de que cada `pte_t` sea válida (con `PTE_V` presente). Esto garantiza que `mprotect` y `munprotect` solo operen sobre páginas válidas.

---

Este README resume los pasos y desafíos enfrentados en la implementación de protección de memoria en xv6.
