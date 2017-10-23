#include <mpi.h>
#include <stdio.h>
#include "group_tags.h"

int main(int argc, char **argv) 
{ 
   MPI_Comm   myComm;       /* intra-communicator of local sub-group */ 
   MPI_Comm   myFirstComm;  /* inter-communicator */ 
   int group_number = 0; 
   int global_rank; 
   int local_rank; 
   MPI_Status status;


   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &global_rank );

   /* Build intra-communicator for local sub-group */ 
   MPI_Comm_split( MPI_COMM_WORLD, // comm
                   group_number,   // color
                   global_rank,    // key
                  &myComm );       // newcomm

   MPI_Comm_rank( myComm, &local_rank );
printf("%s:%d:global_rank %d:local_rank %d\n", __FILE__, __LINE__, global_rank, local_rank);

   /* Build inter-communicators.  Tags are hard-coded. */ 
   /* Group 0 communicates with group 1 */

   MPI_Intercomm_create( myComm,         // local_comm
                         0,              // local_leader
                         MPI_COMM_WORLD, // peer_comm
                         2+local_rank,   // remote_leader
                         1,              // tag
                         &myFirstComm ); // newintercomm

   /* Do work ... */ 
   float numbers[3];

   if (local_rank == 0)
   {
      MPI_Recv( numbers,     // buf
                3,           // count
                MPI_FLOAT,   // datatype
                0,           // source (rank of source)
                TAG_0,       // tag
                myFirstComm, // comm
               &status );    // status
   }
   if (local_rank == 1)
   {
      MPI_Recv( numbers,     // buf
                3,           // count
                MPI_FLOAT,   // datatype
                1,           // source (rank of source)
                TAG_3,       // tag
                myFirstComm, // comm
               &status );    // status
   }

   printf("local rank %d numbers = %f, %f, %f\n", local_rank, numbers[0], numbers[1], numbers[2]);

   // close down stuff
   MPI_Comm_free(&myFirstComm); 
   MPI_Finalize(); 

   return 0;
} 
