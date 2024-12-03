Tarea 5

---

### 1. Lógica y Funcionamiento de la Protección de Memoria

En el sistema operativo **xv6**, las funciones `mprotect` y `munprotect` permiten gestionar los permisos de escritura en páginas específicas de la memoria de un proceso. La lógica de esta protección se basa en la manipulación del bit de escritura (W) dentro de las entradas de la tabla de páginas (PTE) asociadas a las páginas involucradas.

- **`mprotect`:** Esta función modifica el bit W en las PTE de las páginas especificadas para establecerlas como de solo lectura. Recibe como parámetros una dirección de inicio (`addr`) y el tamaño en páginas (`len`). Convierte `addr` a la dirección de inicio de página más próxima y calcula cuántas páginas se deben recorrer. Luego, desactiva el bit de escritura en cada PTE dentro del rango afectado.
  
- **`munprotect`:** Restaura el permiso de escritura en las páginas afectadas, activando nuevamente el bit W en las PTE correspondientes. Los parámetros (`addr` y `len`) se procesan de forma similar a `mprotect`, pero en este caso, se reactiva el bit de escritura.

Cuando un proceso intenta escribir en una página protegida mediante `mprotect`, el sistema genera una excepción de fallo de página, estableciendo un mecanismo efectivo para proteger la memoria.

---

### 2. Cambios Realizados para Implementar las Funciones

Para habilitar `mprotect` y `munprotect` en xv6, se realizaron las siguientes modificaciones en el código:

#### Creación de las Funciones

- **Implementación en `vm.c`:** Se desarrollaron las funciones `mprotect` y `munprotect` dentro de este archivo, que gestiona la memoria en xv6. Ambas funciones recorren las PTE de las páginas indicadas por el rango (`addr` y `len`) y manipulan el bit W: `mprotect` lo desactiva, y `munprotect` lo activa.

#### Incorporación de Nuevas Llamadas al Sistema

- **Definición de Syscalls:** Se añadieron las llamadas `sys_mprotect` y `sys_munprotect` en el archivo `sysproc.c`, para que los procesos puedan acceder a estas funcionalidades. 
- **Declaraciones:** Las nuevas funciones se declararon en `syscall.h` y `user.h`, permitiendo su uso en programas de usuario.

#### Validaciones de Errores

- **Parámetros inválidos:** Se implementaron verificaciones para que las funciones devuelvan un error (`-1`) si `addr` es nulo o si `len` es menor o igual a cero.
- **Rango de direcciones:** Se añadieron validaciones adicionales para garantizar que las direcciones se encuentren dentro del espacio de direcciones del proceso, evitando modificar regiones de memoria no válidas.

---

### 3. Retos y Soluciones Implementadas

#### Manejo de `argaddr` y `argint`

- **Problema:** Durante el desarrollo, se intentó evaluar las funciones `argaddr` y `argint` dentro de condiciones `if`, lo cual resultó en errores ya que estas funciones son de tipo `void` y no devuelven un valor.
- **Solución:** En lugar de intentar comparar los valores de retorno, se llamaron las funciones directamente y se verificaron los valores asignados a las variables correspondientes.

#### Validación de Direcciones

- **Problema:** Comprobar que `addr` y el rango definido por `len` correspondieran a la memoria del proceso fue complejo debido a la gestión de direcciones virtuales en xv6.
- **Solución:** Se añadieron verificaciones para asegurar que las direcciones estuvieran dentro del rango permitido (`MAXVA`) y correctamente alineadas con los límites de página.

#### Pruebas de Fallos de Página

- **Problema:** Las pruebas requerían provocar fallos de página para confirmar el correcto funcionamiento de `mprotect`, lo cual podía detener prematuramente el programa.
- **Solución:** Se optó por comentar temporalmente las secciones de prueba que generaban fallos de página, verificando primero el funcionamiento de `munprotect` y asegurándose de que ambas funciones operaran correctamente en conjunto.