#include <mpi.h>
#include <stdio.h>
#include "group_tags.h"

int main(int argc, char **argv) 
{ 
   MPI_Comm   myComm;       /* intra-communicator of local sub-group */ 
   MPI_Comm   myFirstComm;  /* inter-communicator */ 
   MPI_Comm   mySecondComm; /* second inter-communicator (group 1 only) */ 
   int group_number = 0; 
   int rank; 
   MPI_Status status;


   MPI_Init(&argc, &argv); 
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
printf("%s:%d:rank %d\n", __FILE__, __LINE__, rank);

   /* Build intra-communicator for local sub-group */ 
   MPI_Comm_split( MPI_COMM_WORLD, // comm
                   group_number,   // color
                   rank,           // key
                  &myComm );       // newcomm

   /* Build inter-communicators.  Tags are hard-coded. */ 
   /* Group 0 communicates with group 1 and 2 */

   MPI_Intercomm_create( myComm,         // local_comm
                         0,              // local_leader
                         MPI_COMM_WORLD, // peer_comm
                         1,              // remote_leader
                         1,              // tag
                         &myFirstComm ); // newintercomm

   MPI_Intercomm_create( myComm,          // local_comm
                         0,               // local_leader
                         MPI_COMM_WORLD,  // peer_comm
                         2,               // remote_leader
                         12,              // tag
                         &mySecondComm ); // newintercomm

   /* Do work ... */ 
   float numbers[3];
   MPI_Recv( numbers,     // buf
             3,           // count
             MPI_FLOAT,   // datatype
             0,           // source (rank of source)
             TAG_0,       // tag
             myFirstComm, // comm
            &status );    // status

   printf("numbers = %f, %f, %f\n", numbers[0], numbers[1], numbers[2]);

   // close down stuff
   MPI_Comm_free(&myFirstComm); 
   MPI_Comm_free(&mySecondComm); 
   MPI_Finalize(); 

   return 0;
} 
