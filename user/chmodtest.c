#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


int main(void) {
    int fd = open("testfile", O_CREATE | O_RDWR);
    if (fd < 0) {
        printf("Error creating file\n");
        exit(1);
    }

    printf("File created successfully.\n");

    // Test chmod
    if (chmod("testfile", 1) < 0) {
        printf("chmod failed\n");
        close(fd);
        exit(1);
    }

    printf("chmod succeeded.\n");

    // Close file
    close(fd);

    exit(0);
}
