#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char* ptr[2000];

    for(int i = 0; i < 2000; i++)
    {
        ptr[i] = (char*)malloc(i*10);   
    }

    for(int i = 0; i < 2000; i++)
    {
        if(i%2 == 0)
        {
            free(ptr[i]);
        }
    }



    char* ptr2[30];

    for(int i = 0; i <2000; i++)
    {
        ptr2[i] = (char*)malloc(i*3);
    }

    for(int i = 0; i <2000; i++)
    {
        free(ptr2[i]);
    }


}


