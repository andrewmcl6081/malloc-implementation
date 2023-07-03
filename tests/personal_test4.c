#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char* ptr[2000];

    for(int i = 0; i < 2000; i++)
    {
        if(i %2)
        {
            ptr[i] = (char*)malloc(i*i);
            ptr[i] = ptr[i];
        }
        else
        {
            ptr[i] = (char*)malloc(1);
            ptr[i] = ptr[i];
        }
    }

    for(int i = 0; i < 2000; i++)
    {
        if(i%3)
        {
            free(ptr[i]);
        }
    }

    //lets test it

    for(int i = 0; i < 2000; i++)
    {
        ptr[i] = (char*)malloc(i*2);
        ptr[i] = ptr[i];
    }
}