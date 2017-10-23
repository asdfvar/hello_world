#include <mpi.h>
#include <stdio.h>
#include "group_tags.h"

int main(int argc, char **argv) 
{ 
   MPI_Comm   myComm;       /* intra-communicator of local sub-group */ 
   MPI_Comm   myFirstComm;  /* inter-communicator */ 
   int group_number = 1; 
   int global_rank; 
   int local_rank; 


   MPI_Init(&argc, &argv); 
   MPI_Comm_rank( MPI_COMM_WORLD, &global_rank );

   /* Build intra-communicator for local sub-group */ 
   MPI_Comm_split( MPI_COMM_WORLD, // comm
                   group_number,   // color
                   global_rank,    // key
                  &myComm );       // newcomm

   MPI_Comm_rank( myComm, &local_rank );
printf("%s:%d:global_rank %d:local_rank %d\n", __FILE__, __LINE__, global_rank, local_rank);

   /* Build inter-communicators.  Tags are hard-coded. */ 
   /* Group 1 communicates with group 0 */ 

   MPI_Intercomm_create( myComm,         // local_comm
                         0,              // local_leader
                         MPI_COMM_WORLD, // peer_comm
                         local_rank,     // remote_leader
                         1,              // tag
                        &myFirstComm );  // newintercomm

   /* Do work ... */ 
   float numbers[3] = {10, 12, 13};

   if (local_rank == 0)
   {
      numbers[0] = 0;
      MPI_Send( numbers,       // buf
                3,             // count
                MPI_FLOAT,     // datatype
                0,             // rank of destination
                TAG_0,         // tag
                myFirstComm ); // comm
   }
   if (local_rank == 1)
   {
      numbers[0] = 1;
      MPI_Send( numbers,       // buf
                3,             // count
                MPI_FLOAT,     // datatype
                1,             // rank of destination
                TAG_3,         // tag
                myFirstComm ); // comm
   }

   // close down stuff
   MPI_Comm_free(&myFirstComm); 
   MPI_Finalize(); 

   return 0;
} 
