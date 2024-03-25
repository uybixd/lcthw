#include <stdio.h>

int main(int argc, char *argv[]) {
    int a = 2147483648;
    long b = 2147483647;
    long c = 2147483648;
    unsigned int d = 2147483647;
    d = d *2 + 1;
    long long e = 2147483648;
    long f = 2147483648 + 1;
    long g = 2147483647;
    g++;



    printf("Now let's see which data type can store this large number.\n"
           ">>> int: %d\n"
           "<<<< long: %ld\n"
           "--------\n"
           ">>>> long: %ld\n"
           "<<<< unsigned int: %u\n"
           ">>>> long long: %lld\n",
           a, b, c, d, e);
    printf("----------\n");
    printf("现在做一个新的测试，f在声明变量的时候做一个加法，而g先声明，在下一行做加法。\n");
    printf(">>>> f: %ld\n", f);
    printf("<<<< g: %ld\n", g);

    return 0;
}