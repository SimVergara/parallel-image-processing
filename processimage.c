
#include "a1.h"
#include <string.h>
#include <stdio.h>

void processImage(int width, int height, RGB *image, int my_rank)
{
// simple image "processing"
    int i;
    int my_start = 0;
    int my_end = width*height;
int modran=0;
//printf("P%d: w%d h%d started processing with my_start%d my_end%d\n", 
//    my_rank, width, height, my_start, my_end);

modran = my_rank % 3;
    if(modran==0){
        for (i=my_start; i < my_end; i++)
        {
            RGB *p = image+i;
            //printf("BEF: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
            p->g ++;
            p->b ++;
            p->r = 255;
            //printf("AFT: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
        }
    }
    else if(modran==1){
        for (i=my_start; i < my_end; i++)
        {
            RGB *p = image+i;
            //printf("P1 BEF: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
            p->b = 255;
            p->g ++;
            p->r ++;
            //printf("P1 AFT: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
        }
    }
    else if(modran==2){
        for (i=my_start; i < my_end; i++)
        {
            RGB *p = image+i;
            //printf("P1 BEF: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
            p->g = 255;
            p->b ++;
            p->r ++;
            //printf("P1 AFT: Pixel %d: R%d, G%d, B%d.\n", i,p->r, p->g, p->b);
        }
    }

//    printf("P%d: done processing\n", my_rank);
}
