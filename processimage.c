
#include "a1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void processImage(int width, int height, RGB *image, int n, int filtertype)
{
    int     sumr = 0,//sum of all the RGB values in the pane
            sumb = 0,
            sumg = 0;
    int panecount = 0;//used to count how many 
                      //items are in the NxN pane for the average

    //if (filtertype) RGB *medianbuffer = (RGB*)malloc(9*sizeof(RGB));

    int *medr = (int *)malloc(sizeof(int));
    int *medg = (int *)malloc(sizeof(int));;
    int *medb = (int *)malloc(sizeof(int));;

    int my_start = 0;
    int my_end = width*height;

    int myrow;

    RGB *out = (RGB*)malloc(my_end*sizeof(RGB));

    if (n<2)
    {
        return;
    }


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


                panecount ++;


                if (!filtertype)
                {//if average filter
                    sumr += pi->r;
                    sumb += pi->b;
                    sumg += pi->g;
                }
                else
                {//if median buffer
                    medr = (int *)realloc(medr, panecount*sizeof(int));
                    medg = (int *)realloc(medg, panecount*sizeof(int));
                    medb = (int *)realloc(medb, panecount*sizeof(int));
                    medr[panecount-1] = pi->r;
                    medg[panecount-1] = pi->g;
                    medb[panecount-1] = pi->b;
                }


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
//printf("\npanecount%d\nR\n", panecount);
//printmed(medr, panecount, i);
          q_sort(medr, 0, panecount-1);
//printmed(medr, panecount, i);
//
//printf("G\n");
//printmed(medg, panecount, i);
          q_sort(medg, 0, panecount-1);
//printmed(medg, panecount, i);
//
//printf("B\n");
//printmed(medb, panecount, i);
          q_sort(medb, 0, panecount-1);
//printmed(medb, panecount, i);
//
//
//printf("tt\n");
            if (panecount % 2 == 1)
            {//if odd number of pixels in window
                final->r = medr[(panecount/2)-1];
                final->g = medg[(panecount/2)-1];
                final->b = medb[(panecount/2)-1];
            }
            else
            {//if even number of pixels in window
                final->r = (medr[(panecount/2)-1] + medr[(panecount/2)]) / 2;
                final->g = (medg[(panecount/2)-1] + medg[(panecount/2)]) / 2;
                final->b = (medb[(panecount/2)-1] + medb[(panecount/2)]) / 2;
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


void printmed(int numbers[], int size, int x)
{
    printf("med[%d]: [",x);
    for (int i=0;i<size;i++){
        printf("%d, ", numbers[i] );
    }
    printf("]\n");
}


/*
void q_sort(int numbers[10], int left, int right)
{
    int pivot, l_hold, r_hold;

    l_hold = left;
    r_hold = right;
    pivot = numbers[left];
    while (left < right)
    {
        while ((numbers[right] >= pivot) && (left < right))
            right--;
        if (left != right)
        {
            numbers[left] = numbers[right];
            left++;
        }
        while ((numbers[left] <= pivot) && (left < right))
            left++;
        if (left != right)
        {
            numbers[right] = numbers[left];
            right--;
        }
    }
    numbers[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot)
        q_sort(numbers, left, pivot-1);
    if (right > pivot)
        q_sort(numbers, pivot+1, right);
}*/




void q_sort( int a[], int l, int r)
{
   int j;

   if( l < r ) 
   {
    // divide and conquer
        j = partition( a, l, r);
       q_sort( a, l, j-1);
       q_sort( a, j+1, r);
   }
    
}



int partition( int a[], int l, int r) {
   int pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;
        
   while( 1)
   {
    do ++i; while( a[i] <= pivot && i <= r );
    do --j; while( a[j] > pivot );
    if( i >= j ) break;
    t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}
