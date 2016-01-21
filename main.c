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
	int 		my_height, 
				my_width;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (my_rank == 0){
		RGB *image;

		int width, height, max;

		//reads photo
		image = readPPM(argv[1], &width, &height, &max);
printf("P0: Image Loaded\nW%d H%d M%d\n", width, height, max );
		//split up photo in sections

		//send sections to each process
		long int imsending = width*((height)/2);

		my_width = width;
		my_height = height;//p;

		int msglength = my_width*my_height;

		MPI_Send(&my_width, sizeof(int), MPI_CHAR, 1, 1, MPI_COMM_WORLD);
		MPI_Send(&my_height, sizeof(int), MPI_CHAR, 1, 1, MPI_COMM_WORLD);
		
		MPI_Send(image+imsending, msglength*1.5, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
		printf("P0: send whole image to P1 of W%d H%d Sz%d\n", 
												my_width, 
												my_height,
												msglength);

		//process first section
		processImage(width, (height), image, my_rank);

		//receive from processes
		MPI_Recv(image+imsending, msglength*1.5, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
		
		writePPM(argv[2], width, height, max, image);

		free(image);
	}
	else{
		source = 0;
		MPI_Recv(&my_width, sizeof(int), MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&my_height, sizeof(int), MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);
printf("P%d: received parameters H%d W%d\n", my_rank, my_height, my_width);

		int msglength = my_width*my_height;
		RGB *image1 = (RGB*)malloc(msglength*sizeof(RGB));
		MPI_Recv(image1, msglength*1.5, MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);


/*printf("Before0 : R%d G%d B%d\n", image1->r, image1->g, image1->b);
printf("Before10: R%d G%d B%d\n", (image1+10)->r, (image1+10)->g, (image1+10)->b);
printf("BeforeL : R%d G%d B%d\n", (image1+my_width*my_height/400)->r,
								  (image1+my_width*my_height/400)->g, 
								  (image1+my_width*my_height/400)->b);
*/		processImage(my_width, my_height, image1, my_rank);
/*printf("After0 : R%d G%d B%d\n", image1->r, image1->g, image1->b);
printf("After10: R%d G%d B%d\n", (image1+10)->r, (image1+10)->g, (image1+10)->b);
printf("AfterL : R%d G%d B%d\n", (image1+my_width*my_height)->r,
								  (image1+my_width*my_height)->g, 
								  (image1+my_width*my_height)->b);

*/		MPI_Send(image1, msglength*1.5, MPI_CHAR, 0, 0, MPI_COMM_WORLD);		

	}


printf("P%d finished\n", my_rank);
	MPI_Finalize();

	return 0;
}



/*
for splitting the image;

height/p + i < height%p ? 1 : 0;
*/
