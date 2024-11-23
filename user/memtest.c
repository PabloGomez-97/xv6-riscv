#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test_protection(void)
{
    // Reservar 2 páginas de memoria
    char *addr = sbrk(2 * 4096);
    if (addr == (char *)-1)
    {
        printf("Error: sbrk falló\n");
        exit(1);
    }

    // Escribir un valor inicial
    addr[0] = 'A';
    printf("Valor inicial: %c\n", addr[0]);

    // Proteger la primera página
    if (mprotect(addr, 1) < 0)
    {
        printf("Error: mprotect falló\n");
        exit(1);
    }
    printf("Página protegida exitosamente\n");

    // Intentar escribir en la página protegida (debería causar un fallo)
    printf("Intentando escribir en página protegida...\n");
    addr[0] = 'B'; // Esto debería causar un page fault

    // El código siguiente no debería ejecutarse debido al page fault
    printf("Error: La protección de memoria falló\n");
    exit(1);
}

int main(void)
{
    printf("Iniciando prueba de protección de memoria...\n");
    test_protection();
    return 0;
}