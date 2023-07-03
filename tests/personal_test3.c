#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int* ptr[500];

    for(int i = 0; i < 500; i++)
    {
        ptr[i] = (int*)malloc(i * sizeof(int));
    }

    for(int i = 0; i < 500; i++)
    {
        if(i % 2)
        {
            free(ptr[i]);
        }
    }

    char * ptr1 = ( char * ) malloc ( 2000 );
    char * buf1 = ( char * ) malloc ( 1 );
    char * ptr6= ( char * ) malloc ( 10 );
    char * buf2 = ( char * ) malloc ( 1 );
    char * ptr2 = ( char * ) malloc ( 6000 );
    char * buf3 = ( char * ) malloc ( 1 );
    char * ptr7= ( char * ) malloc ( 10 );
    char * buf4 = ( char * ) malloc ( 1 );
    char * ptr3 = ( char * ) malloc ( 1000 );

    ptr1 = ptr1;
    buf1 = buf1;
    ptr6 = ptr6;
    buf2 = buf2;
    ptr2 =ptr2;
    buf3 = buf3;
    ptr7 = ptr7;
    buf4 = buf4;
    ptr3 = ptr3;

    int* int_ptr[500];

    for(int i = 0; i < 500; i++)
    {
        int_ptr[i] = (int*) malloc(i * sizeof(int));
        int_ptr[i] = int_ptr[i];
    }
}