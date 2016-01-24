# parallel-image-processing
An introductory sample to get started with distributed systems.

This is a school project for a Distributed Systems course. The point of it is to create a program which will take in several parameters and apply a filter to the image. The image is processed in different parts by MPI processes

The program can be run by using: mpirun -np p ./ppmf photo.ppm out.ppm

where p is number of processes.
      photo.ppm is the input photo in ppm format.
      out.ppm is the output photo. will be ppm format.

OpenMPI needed for this to work properly