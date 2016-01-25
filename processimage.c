
#include "a1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void processImage(int width, int height, RGB *image, int n, int filtertype)
{
    double  sumr = 0,//sum of all the RGB values in the pane
            sumb = 0,
            sumg = 0;
    int panecount = 0;//used to count how many 
                      //items are in the NxN pane for the average

    //if (filtertype) RGB *medianbuffer = (RGB*)malloc(9*sizeof(RGB));

    int medr[9];
    int medg[9];
    int medb[9];

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

                RGB *pi = po - y*width - x;

                if (!filtertype)
                {//if average filter
                    sumr += pi->r;
                    sumb += pi->b;
                    sumg += pi->g;
                }
                else
                {//if median buffer
                    medr[panecount] = pi->r;
                    medg[panecount] = pi->g;
                    medb[panecount] = pi->b;
                }


                panecount ++;

            }//rows
        }//columns





        RGB *final = out+i;
        if(!filtertype)
        {
            final->r = sumr/(panecount);
            final->g = sumg/(panecount);
            final->b = sumb/(panecount);
        }
        else
        {
            quick_sort(medr,0);
            if (panecount % 2 == 1)
            {//if odd number of pixels in window

            }
            else
            {//if even number of pixels in window

            }
        }

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



void quick_sort (int *a, int n) {
    int i, j, p, t;
    if (n < 2)
        return;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i] < p)
            i++;
        while (p < a[j])
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    quick_sort(a, i);
    quick_sort(a + i, n - i);
}
