#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main()
{
    const char *filename = "testfile";

    // Creación del archivo con permisos de lectura y escritura
    int fd = open(filename, O_CREATE | O_RDWR);
    if (fd < 0)
    {
        printf("Error: no se pudo crear el archivo.\n");
        exit(1);
    }
    printf("Archivo creado exitosamente con permisos de lectura/escritura.\n");

    // Escritura inicial
    if (write(fd, "Test", 4) != 4)
    {
        printf("Error: no se pudo escribir en el archivo.\n");
        close(fd);
        exit(1);
    }
    printf("Escritura inicial completada.\n");
    close(fd);

    // Cambio de permisos a solo lectura
    if (chmod(filename, 1) < 0)
    {
        printf("Error: chmod a solo lectura falló.\n");
        exit(1);
    }
    printf("Permisos cambiados a solo lectura.\n");

    // Intento de abrir en modo escritura
    fd = open(filename, O_WRONLY);
    if (fd >= 0)
    {
        printf("Error: se pudo abrir el archivo en modo escritura cuando debería estar en solo lectura.\n");
        close(fd);
        exit(1);
    }
    printf("El archivo no puede abrirse en modo escritura (solo lectura activo).\n");

    // Cambio de permisos a lectura/escritura
    if (chmod(filename, 3) < 0)
    {
        printf("Error: chmod a lectura/escritura falló.\n");
        exit(1);
    }
    printf("Permisos cambiados a lectura/escritura.\n");

    // Escritura final
    fd = open(filename, O_RDWR);
    if (fd < 0)
    {
        printf("Error: no se pudo abrir el archivo en modo lectura/escritura.\n");
        exit(1);
    }
    if (write(fd, " Final Test", 11) != 11)
    {
        printf("Error: no se pudo escribir en el archivo después de restaurar los permisos.\n");
        close(fd);
        exit(1);
    }
    printf("Escritura final completada exitosamente.\n");
    close(fd);

    exit(0);
}
