#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) 
{ 
   MPI_Comm   myComm;       /* intra-communicator of local sub-group */ 
   MPI_Comm   myFirstComm;  /* inter-communicator */ 
   MPI_Comm   mySecondComm; /* second inter-communicator (group 1 only) */ 
   int group_number = 0; 
   int rank; 


   MPI_Init(&argc, &argv); 
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

   /* Build intra-communicator for local sub-group */ 
   MPI_Comm_split(MPI_COMM_WORLD, group_number, rank, &myComm); 

   /* Build inter-communicators.  Tags are hard-coded. */ 
   /* Group 0 communicates with group 1 and 2 */
   MPI_Intercomm_create( myComm, 0, MPI_COMM_WORLD, 1, 1, &myFirstComm); 
   MPI_Intercomm_create( myComm, 0, MPI_COMM_WORLD, 2, 12, &mySecondComm); 

   /* Do work ... */ 
   printf("%s: hello\n", __FILE__);

   // close down stuff
   MPI_Comm_free(&myFirstComm); 
   MPI_Comm_free(&mySecondComm); 
   MPI_Finalize(); 

   return 0;
} 
