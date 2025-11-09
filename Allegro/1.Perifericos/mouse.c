// sudo gcc mouse.c -o mouse

#include <stdio.h>
#include <stdlib.h>


int main(){
    FILE *mouse = fopen("/dev/input/mice", "rb");

    if (!mouse){
        return 1;
    }

    char a;
    while(1){
        a = fgetc(mouse);
        printf("%d\n", a);
    }

    fclose(mouse);
    return 0;
}