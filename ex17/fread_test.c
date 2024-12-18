#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *file;
    int numbers[5];

    file = fopen("numbers.bin", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    // here should use size_t, an unsigned int
    int result = fread(numbers, sizeof(int), 5, file);
    if (result != 5) {
        perror("Error reading file");
    } else {
        for (int i = 0; i < 5; i++) {
            printf("Number %d: %d\n", i + 1, numbers[i]);
        }
    }

    fclose(file);
    return 0;
}