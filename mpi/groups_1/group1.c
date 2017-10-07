#include <mpi.h>
#include <stdio.h>

main(int argc, char **argv) 
{ 
   MPI_Comm   myComm;       /* intra-communicator of local sub-group */ 
   MPI_Comm   myFirstComm;  /* inter-communicator */ 
   int group_number = 1; 
   int rank; 


   MPI_Init(&argc, &argv); 
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

   /* Build intra-communicator for local sub-group */ 
   MPI_Comm_split(MPI_COMM_WORLD, group_number, rank, &myComm); 


   /* Build inter-communicators.  Tags are hard-coded. */ 
   /* Group 1 communicates with group 0 */ 
   MPI_Intercomm_create( myComm, 0, MPI_COMM_WORLD, 0, 1, &myFirstComm); 


   /* Do work ... */ 

   printf("%s: hello\n", __FILE__);

   // close down stuff
   MPI_Comm_free(&myFirstComm); 
   MPI_Finalize(); 

   return 0;
} 

