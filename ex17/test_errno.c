#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    //if (read(fd, buf, 1) == -1) {
        //printf("Oh dear, something wen wrong with read()! %s\n", strerror(errno));
    //}
    FILE *file = fopen("nonexistent_file.txt", "r");
    if (!file) {
        printf("Error opening file: %s\n", strerror(errno));
        perror("TEST");
        printf(">>> %d\n", errno);
    }

    return 0;
}