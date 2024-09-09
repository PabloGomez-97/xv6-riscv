#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(void)
{
    printf("Mi padre es: %d\n", getppid());
    printf("Mi abuelo es: %d\n", getancestor(23));
    exit(0);
}