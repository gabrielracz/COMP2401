#include "stdio.h"
#include "stdlib.h"

// This is a forward reference
int fact(int n);

int main(int argc, char **argv)

{
    int sum = fact(5);
    printf("fact(5) = %d\n",sum);

    return(0);
}

int fact(int n) {
    int sum;
    if (n ==1) {
        return(n);
    }
    sum = fact(n-1);
    sum *= n;
    return(sum);
}

