#include <stdio.h>

int main(int argc, char *argv[])
{
    // Calculate each struct size
    struct Test1 {
        char a;
        int b;
        short c;
    };

    int size1;
    size1 = sizeof(struct Test1);

    printf("The sizeof Test1 should be 12 : %d\n", size1);

    printf("The sizeof long is: %lu\n", sizeof(long));

    struct Test2 {
        char a;
        char b;
        int c;
    };
    int size2;
    size2 = sizeof(struct Test2);

    printf("The sizeof Test2 should be 8 : %d\n", size2);
    

    return 0;
}