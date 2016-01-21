
#include "a1.h"
#include <string.h>
#include <stdio.h>

void processImage(int width, int height, RGB *image, int my_rank)
{
// simple image "processing"
    int i;
    int my_start = 0;
    int my_end = width*height;

printf("P%d: w%d h%d --- my_start%d my_end%d\n", 
    my_rank, width, height, my_start, my_end);

if(my_rank==0){
    for (i=my_start; i < my_end; i++)
    {
        RGB *p = image+i;
        //printf("BEF: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
        //p->g = 0;//p->b%128;
        //p->b = 0;//p->r%128;
        p->r = 255;//p->g%128;
        //printf("AFT: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
    }
}
else{
    int tb;//,tr, tg;
    for (i=my_start; i < my_end; i++)
    {
        RGB *p = image+i;
        //printf("P1 BEF: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
        tb= 255;//p->b%128;
        /*tr= p->r%128;
        tg= p->g%128;
        p->g = tg;
        p->r = tr;*/
        p->b = tb;
        //printf("P1 AFT: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
    }
}

    printf("P%d: done processing\n", my_rank);
}
