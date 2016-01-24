
#include "a1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void processImage(int width, int height, RGB *image, int my_rank)
{
    double  sumr = 0,//sum of all the RGB values in the pane
            sumb = 0,
            sumg = 0;
    int panecount = 0;//used to count how many 
                      //items are in the NxN pane for the average

    int my_start = 0;
    int my_end = width*height;

    int myrow;
    int n = 3;

    RGB *out = (RGB*)malloc(my_end*sizeof(RGB));

    for (int i = my_start; i < my_end; i++)
    {//loops through each pixel
        panecount = 0;
        sumr = 0;
        sumb = 0;
        sumg = 0;

        RGB *po = image + i;

        for (int y = (n/2); y > ((n/2)-n); y--)
        {//loops columns
            for (int x = (n/2); x > ((n/2)-n) ; x--)
            {//loops rows
                RGB *pi = po - y*width - x;
//printf("R%d G%d B%d\n", pi->r, pi->g, pi->b);
                sumr += pi->r;
                sumb += pi->b;
                sumg += pi->g;
            }//rows
        }//columns

//printf("sumr%d sumg%d sumb%d\n", sumr,sumg,sumb);


        RGB *final = out+i;
        final->r = sumr/(n*n);
        final->g = sumg/(n*n);
        final->b = sumb/(n*n);



    }//pixels

     for (int i = my_start; i < my_end; i++)
     {
        RGB *temp = out+i;
        RGB *last = image+i;

        last->r = temp->r;
        last->g = temp->g;
        last->b = temp->b;
     }

}















void dumbfunct(int width, int height, RGB *image, int my_rank)
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
