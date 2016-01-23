#include <stdlib.h>
#include "a1.h"
#include "mpi.h"
#include <string.h>

int main(int argc, char** argv)
{
	int 		my_rank, p, source;//, dest;
	int 		tag = 0;
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

	if (my_rank == 0){
		RGB *image;

		int width, height, max;

		//reads photo
		image = readPPM(argv[1], &width, &height, &max);


		my_width = width;

		my_height = (int*)malloc(sizeof(int)*p);

		for (int i=0; i<p; i++){
			//send sections to each process

			my_height[i] = height/p;
			if (i < height%p) my_height[i]++;
		
			msglength = my_width * my_height[i];
			
			if (i>0){
				offset = 0;
				for (int j=0; j<i;j++){
					offset += width * my_height[j];	
				}
printf("P%d: sending    image of W%d H%d 	size %d to P%d\n", my_rank, my_width,
														my_height[i], msglength, i);
				
				MPI_Send(&my_width, sizeof(int), MPI_CHAR, i, 1, MPI_COMM_WORLD);
				MPI_Send(&my_height[i], sizeof(int), MPI_CHAR, i, 1, MPI_COMM_WORLD);
				
				MPI_Send(image+offset, msglength/**3*/, MPI_CHAR, i, 1, MPI_COMM_WORLD);

			}
		}
		//process first section
		processImage(width, (my_height[0]), image, my_rank);

		//receive from processes
		
		for (int i=1; i<p; i++){
			msglength = my_width * my_height[i];

			offset = 0;
				for (int j=0; j<i;j++){
					offset += width * my_height[j];	
				}

			MPI_Recv(image+offset, msglength/**3*/, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
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
//printf("P%d: received parameters H%d W%d\n", my_rank, my_height, my_width);
		
		int hh = my_height;
		int msglength = hh * my_width;
//printf("P%d: MESSAGELENGTH %d\n", my_rank, msglength);
		RGB *image1 = (RGB*)malloc(msglength*sizeof(RGB));
		MPI_Recv(image1, msglength/**3*/, MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);

printf("P%d: processing image of W%d H%d 	size %d\n", my_rank, my_width,
														my_height, msglength);

RGB *p = image1+msglength;
p->r=255;
p->b=255;
p->g=255;
		processImage(my_width, hh, image1, my_rank);

		MPI_Send(image1, msglength/**3*/, MPI_CHAR, 0, 0, MPI_COMM_WORLD);		

	}


//printf("P%d finished\n", my_rank);
	MPI_Finalize();

	return 0;
}



/*
for splitting the image;

myh = height/p + i < height%p ? 1 : 0;
*/
