#include <stdio.h>  

#define ASCIIMAX 127

int main()  
{  
    int i;  

    for( i=0 ; i<= ASCIIMAX ; i++ ) /*ASCII values ranges from 0-127*/  
    {  
        printf("ASCII value of character %c = %d\n", i, i);  
    }  

    return 0;  
}   
