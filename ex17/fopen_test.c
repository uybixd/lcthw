#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *file;
    
    file = fopen("fread_test.c", "r");

    if (file == NULL) {
        perror("Error opening file");

        return 1;
    }

    printf("File Opend sucessfully.\n");
    printf("The size of stream file: %lu\n", sizeof(file));

    fclose(file);

    return(0);
}