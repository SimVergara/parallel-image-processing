
#include "a1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void processImage(int width, int height, RGB *image, int n)
{
    double  sumr = 0,//sum of all the RGB values in the pane
            sumb = 0,
            sumg = 0;
    int panecount = 0;//used to count how many 
                      //items are in the NxN pane for the average

    int my_start = 0;
    int my_end = width*height;

    int myrow;

    RGB *out = (RGB*)malloc(my_end*sizeof(RGB));

    for (int i = my_start; i < my_end; i++)
    {//loops through each pixel
        panecount = 0;
        sumr = 0;
        sumb = 0;
        sumg = 0;

        RGB *po = image + i;


        for (int y = (n/2); y > ((n/2)-n); y--)
        {//loops within columns
            myrow = (i-y*width)/width;
            for (int x = (n/2); x > ((n/2)-n) ; x--)
            {//loop within rows
                if (((i-y*width - x) < 0) || ((i-y*width - x) >= my_end)) continue;

                int testrow = (i-y*width - x)/width;

                if (myrow != testrow) continue;

                panecount ++;
                RGB *pi = po - y*width - x;

                sumr += pi->r;
                sumb += pi->b;
                sumg += pi->g;
            }//rows
        }//columns


//printf("i%d panecount%d\n", i,panecount);

        RGB *final = out+i;
        final->r = sumr/(panecount);
        final->g = sumg/(panecount);
        final->b = sumb/(panecount);

    }//pixels

     for (int i = my_start; i < my_end; i++)
     {
        RGB *temp = out+i;
        RGB *last = image+i;

        last->r = temp->r;
        last->g = temp->g;
        last->b = temp->b;
     }

    free(out);
    

}



