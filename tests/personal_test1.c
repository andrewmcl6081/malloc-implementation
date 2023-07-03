#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char * ptr1 = ( char * ) malloc ( 10 );
    char * buf1 = ( char * ) malloc ( 1 );
    char * ptr2= ( char * ) malloc ( 300 );
    char * buf2 = ( char * ) malloc ( 1 );
    char * ptr3 = ( char * ) malloc ( 5 );
    char * buf3 = ( char * ) malloc ( 1 );
    char * ptr4= ( char * ) malloc ( 200 );
    char * buf4 = ( char * ) malloc ( 1 );
    char * ptr5 = ( char * ) malloc ( 500 );
    char * buf5 = ( char * ) malloc ( 1 );
    char * ptr6 = ( char * ) malloc ( 5000 );
    char * buf6 = ( char * ) malloc ( 1 );
    char * ptr7= ( char * ) malloc ( 3000 );
    char * buf7 = ( char * ) malloc ( 1 );
    char * ptr8 = ( char * ) malloc ( 300 );
    char * buf8 = ( char * ) malloc ( 1 );
    char * ptr9= ( char * ) malloc ( 875 );
    char * buf9 = ( char * ) malloc ( 1 );
    char * ptr10 = ( char * ) malloc ( 444 );

    ptr10 = ptr10;
    buf9 = buf9;
    ptr9 = ptr9;
    buf8 = buf8;
    ptr8 = ptr8;
    buf7 = buf7;
    ptr7 = ptr7;
    buf6 = buf6;
    ptr6 = ptr6;
    buf5 = buf5;
    ptr5 = ptr5;
    buf4 = buf4;
    ptr4 = ptr4;
    buf3 = buf3;
    ptr3 = ptr3;
    buf2 = buf2;
    ptr2 = ptr2;
    ptr2 = ptr2;
    buf1 = buf1;
    ptr1 = ptr1;

    free(ptr3);
    free(ptr7);
    free(ptr1);
    free(ptr6);

    char* ptr[10];

    for(int i = 0; i < 10; i++)
    {
        ptr[i] = (char*)malloc(i%543);
    }

    for(int i = 0; i < 10; i++)
    {
        free(ptr[i]);
    }

    free(ptr10);
    free(buf9);
    free(ptr9);
    free(buf8);
    free(ptr8);
    free(buf7);
    free(buf6);
    free(buf5);
    free(ptr5);
    free(buf4);
    free(ptr4);
    free(buf3);
    free(ptr2);
    free(buf2);
    free(buf1);
}