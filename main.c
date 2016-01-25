#include <stdlib.h>
#include "a1.h"
#include "mpi.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int 		my_rank, p, source;//, dest;
//	char 		message[20000];
	MPI_Status	status;
//	int 		msglength=20000;
	int 		*my_height, 
				my_width,
				msglength;
	long int 	offset;


	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	MPI_Comm_size(MPI_COMM_WORLD, &p);

	//check if correct input
	if (  (argc != 5) || (atoi(argv[3])==0) || (atoi(argv[3]) % 2 != 1) || ((argv[4][0] != 'M') && (argv[4][0] != 'A'))  )
	{
		if (my_rank==0) printf("\nUsage: ./ppmf inputfile outputfile windowsize filtertype\
			windowsize must be an odd number > 0\nfiltertype: A - average. M - median\n\n");
		MPI_Finalize();
		return 0;
	}

	int 		n = atoi(argv[3]);
	int 		filtertype = 0;

	if (argv[4][0] == 'M') filtertype = 1;


	if (my_rank == 0){
		RGB *image;

		int width, height, max;


		//reads photo
		image = readPPM(argv[1], &width, &height, &max);

		int procoffset = (n/2)*width;
		int msgoffset = (n/2)*2;

		my_width = width;

		my_height = (int*)malloc(sizeof(int)*p);

		for (int i=0; i<p; i++){
			//send sections to each process

			my_height[i] = height/p;
			if (i < height%p) my_height[i]++;
			
			if (i>0){

				if (i==(p-1)) msgoffset = n/2;
				msglength = my_width * (my_height[i] + msgoffset);

				offset = 0;
				for (int j=0; j<i;j++){
					offset += width * my_height[j];	
				}
				
				MPI_Send(&my_width, sizeof(int), MPI_CHAR, i, 1, MPI_COMM_WORLD);
				MPI_Send(&my_height[i], sizeof(int), MPI_CHAR, i, 1, MPI_COMM_WORLD);
				

				MPI_Send(image + offset - procoffset, 
						 (msglength)*3, 
						 MPI_CHAR, 
						 i, 
						 1, 
						 MPI_COMM_WORLD);
			}
		}

		//when only one process, don't need an offset
		if (p == 1) msgoffset = 0 ;


		processImage(width, (my_height[0]+msgoffset), image, n, filtertype);
		

		//receive from processes
		for (int i=1; i<p; i++){
			msglength = my_width * my_height[i];

			offset = 0;
				for (int j=0; j<i;j++){
					offset += width * my_height[j];	
				}

			MPI_Recv(image+offset, msglength*3, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
		}
		
		writePPM(argv[2], width, height, max, image);

		free(image);
	}
	else //Every other rank will do this
	{

		my_height = (int*)malloc(sizeof(int));
		source = 0;
		MPI_Recv(&my_width, sizeof(int), MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&my_height, sizeof(int), MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);

		int procoffset = (n/2)*my_width;
		int msgoffset = (n/2)*2;

		if (my_rank==(p-1)) msgoffset = n/2;

		int hh = my_height;
		int msglength = (hh + msgoffset) * my_width;
		RGB *image1 = (RGB*)malloc(msglength*sizeof(RGB));

		MPI_Recv(image1, (msglength)*3, MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);


		processImage(my_width, hh + msgoffset, image1, n, filtertype);
		


		msglength = hh * my_width;

		MPI_Send(image1 + procoffset, msglength*3, MPI_CHAR, 0, 0, MPI_COMM_WORLD);		

		free(image1);
	}


	MPI_Finalize();

	return 0;
}



/*
for splitting the image;

myh = height/p + i < height%p ? 1 : 0;
*/
