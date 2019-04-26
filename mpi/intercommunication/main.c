#include "mpi.h"
#include <stdio.h>

enum { false, true };
enum { EX_TAG, INTERCOMM_TAG, INTERCOMM_P2P };

int main (int argc, char* argv[]) {

   int numprocs, rank;

   MPI_Init (&argc, &argv);
   MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank (MPI_COMM_WORLD, &rank);

   printf ("world procs = %d, world rank = %d\n", numprocs, rank);

   MPI_Group group1, group2, group3;
   MPI_Comm comm1;
   MPI_Comm comm2;
   MPI_Comm comm3;
   MPI_Group groupWorld;
   int ranks01[2] = {0, 1};
   int ranks23[2] = {2, 3};
   int ranks12[2] = {1, 2};

   // get the universal group from MPI_COMM_WORLD
   MPI_Comm_group (MPI_COMM_WORLD, &groupWorld);

   // create groups from the universal group
   MPI_Group_incl (groupWorld, 2, ranks01, &group1);
   MPI_Group_incl (groupWorld, 2, ranks23, &group2);
   MPI_Group_incl (groupWorld, 2, ranks12, &group3);

   // show that the first group size = 2 irregardless of the rank
   int size;
   MPI_Group_size (group1, &size);
   printf ("group1 size = %d from rank %d\n", size, rank);

   // get the relative rank number of the first group
   int g1_rank;
   MPI_Group_rank (group1, &g1_rank);

   // get the relative rank number of the second group
   int g2_rank;
   MPI_Group_rank (group2, &g2_rank);

   // get the relative rank number of the third group
   int g3_rank;
   MPI_Group_rank (group3, &g3_rank);
   if (g3_rank == MPI_UNDEFINED) {
      printf ("third group rank is undefined for absolute rank %d\n", rank);
   } else {
      printf ("group rank = %d for absolute rank %d\n", g3_rank, rank);
   }

   // union groups 1 and 3
   MPI_Group group1u3;
   int g1u3_rank;
   MPI_Group_union (group1, group3, &group1u3);
   MPI_Group_rank (group1u3, &g1u3_rank);
   if (g1u3_rank != MPI_UNDEFINED) printf ("group1u3 contains absolute rank %d via relative rank %d\n", rank, g1u3_rank);

   // union groups 1 and 2
   MPI_Group group1u2;
   int g1u2_rank;
   MPI_Group_union (group1, group2, &group1u2);
   MPI_Group_rank (group1u2, &g1u2_rank);
   if (g1u2_rank != MPI_UNDEFINED) printf ("group1u2 contains absolute rank %d via relative rank %d\n", rank, g1u2_rank);

   // intersect groups 1 and 3
   MPI_Group group1n3;
   int g1n3_rank;
   MPI_Group_intersection (group1, group3, &group1n3);
   MPI_Group_rank (group1n3, &g1n3_rank);
   if (g1n3_rank != MPI_UNDEFINED) printf ("group1n3 contains absolute rank %d via relative rank %d\n", rank, g1n3_rank);

   // create a communicator for the first group
   MPI_Comm_create (MPI_COMM_WORLD, group1, &comm1);

   // create a communicator for the second group
   MPI_Comm_create (MPI_COMM_WORLD, group2, &comm2);

   // create a communicator for the third group
   MPI_Comm_create (MPI_COMM_WORLD, group3, &comm3);

   // display that this is a null communicator for the other ranks not in group 1
   if (comm1 == MPI_COMM_NULL) {
      printf ("comm1 is a null communicator for rank %d\n", rank);
   }

   // intra-communication within group 3
   float pi = 3.141592f;
   float e  = 2.718281f;
   float buf;
   MPI_Status stat;
   if (g3_rank == 0) MPI_Send (&pi, 1, MPI_FLOAT, 1, EX_TAG, comm3);
   if (g3_rank == 1) {
      MPI_Recv (&buf, 1, MPI_FLOAT, 0, EX_TAG, comm3, &stat);
      printf ("group 3 rank 1 received pi = %f\n", buf);
   }

   // identify this communicator as an intra-communicator
   int test;
   if (g1_rank != MPI_UNDEFINED) MPI_Comm_test_inter (comm1, &test);
   if (g1_rank != MPI_UNDEFINED) if (test == true)  printf ("communicator 1 is an inter-communicator\n");
   if (g1_rank != MPI_UNDEFINED) if (test == false) printf ("communicator 1 is an intra-communicator\n");

   // create intra-communicator for group 1 union 2
   MPI_Comm intraComm1u2;
   MPI_Comm_create (MPI_COMM_WORLD, group1u2, &intraComm1u2);

   // verify that the intra-communicator is an intra-communicator
   if (g1_rank != MPI_UNDEFINED) MPI_Comm_test_inter (intraComm1u2, &test);
   if (g1_rank != MPI_UNDEFINED) if (test == true)  printf ("intra-communicator 1 union 2 is an inter-communicator\n");
   if (g1_rank != MPI_UNDEFINED) if (test == false) printf ("intra-communicator 1 union 2 is an intra-communicator\n");

   MPI_Comm interComm1to2;
   MPI_Comm interComm2to1;

   // group 1 communicates with group 2
<<<<<<< HEAD
   if (g1_rank != MPI_UNDEFINED) MPI_Intercomm_create (comm1, 0, MPI_COMM_WORLD, 2, INTERCOMM_TAG, &interComm1to2);
   if (g1_rank != MPI_UNDEFINED) {
      printf ("creating inter communicator from world rank %d from communicator containing ranks starting from %d to starting world rank %d at tag %d\n", rank, g1_rank, 2, INTERCOMM_TAG);
   }
=======
   if (g1_rank != MPI_UNDEFINED) MPI_Intercomm_create (comm1, 0, intraComm1u2, 2, INTERCOMM_TAG, &interComm1to2);
>>>>>>> cc6cf49346756616846e4243688a8f1c933be06b

   // group 2 communicates with group 1
   if (g2_rank != MPI_UNDEFINED) MPI_Intercomm_create (comm2, 0, intraComm1u2, 0, INTERCOMM_TAG, &interComm2to1);

   // identify this communicator as an inter-communicator
   if (g1_rank != MPI_UNDEFINED) MPI_Comm_test_inter (interComm1to2, &test);
   if (g1_rank != MPI_UNDEFINED) if (test == true)  printf ("communicator 1 to 2 is an inter-communicator\n");
   if (g1_rank != MPI_UNDEFINED) if (test == false) printf ("communicator 1 to 2 is an intra-communicator\n");

   // identify this communicator as an intra-communicator
   if (g2_rank != MPI_UNDEFINED) MPI_Comm_test_inter (interComm2to1, &test);
   if (g2_rank != MPI_UNDEFINED) if (test == true)  printf ("communicator 2 to 1 is an inter-communicator\n");
   if (g2_rank != MPI_UNDEFINED) if (test == false) printf ("communicator 2 to 1 is an intra-communicator\n");

   // send a value from group 1 to group 2 via inter-communication between groups
   buf = 0.0f;
   if (g1_rank == 0) MPI_Send (&e,   1, MPI_FLOAT, 0, INTERCOMM_P2P, interComm1to2);
   if (g2_rank == 0) MPI_Recv (&buf, 1, MPI_FLOAT, 0, INTERCOMM_P2P, interComm2to1, &stat);
   if (g2_rank == 0) printf ("group 2 relative rank 0 received e = %f from group 1 relative rank 0\n", buf);

   // verify that the group associated with the inter communicators are identical to their intra-communicator counterparts
   MPI_Group testGroup;
   if (g1_rank != MPI_UNDEFINED) MPI_Comm_group (interComm1to2, &testGroup);
   if (g1_rank != MPI_UNDEFINED) MPI_Group_compare (group1, testGroup, &test);
   if (g1_rank != MPI_UNDEFINED && test == MPI_IDENT)   printf ("the group for inter communication handle from group 1 to 2 is identical to communicator 1's group\n");
   if (g1_rank != MPI_UNDEFINED && test == MPI_SIMILAR) printf ("the group for inter communication handle from group 1 to 2 is similar to communicator 1's group\n");
   if (g1_rank != MPI_UNDEFINED && test == MPI_UNEQUAL) printf ("the group for inter communication handle from group 1 to 2 is unequal to communicator 1's group\n");

   if (g2_rank != MPI_UNDEFINED) MPI_Comm_group (interComm2to1, &testGroup);
   if (g2_rank != MPI_UNDEFINED) MPI_Group_compare (group2, testGroup, &test);
   if (g2_rank != MPI_UNDEFINED && test == MPI_IDENT)   printf ("the group for inter communication handle from group 2 to 1 is identical to communicator 2's group\n");
   if (g2_rank != MPI_UNDEFINED && test == MPI_SIMILAR) printf ("the group for inter communication handle from group 2 to 1 is similar to communicator 2's group\n");
   if (g2_rank != MPI_UNDEFINED && test == MPI_UNEQUAL) printf ("the group for inter communication handle from group 2 to 1 is unequal to communicator 2's group\n");

   // free the groups
   MPI_Group_free (&group1);
   MPI_Group_free (&group2);
   MPI_Group_free (&group3);
   MPI_Group_free (&group1u3);
   MPI_Group_free (&group1n3);
   MPI_Group_free (&testGroup);

   // free the communicator handles
   if (g1_rank != MPI_UNDEFINED) {
      MPI_Comm_free (&comm1);
      MPI_Comm_free (&interComm1to2);
   }
   if (g2_rank != MPI_UNDEFINED) {
      MPI_Comm_free (&comm2);
      MPI_Comm_free (&interComm2to1);
   }

   if (g3_rank != MPI_UNDEFINED) {
      MPI_Comm_free (&comm3);
   }

   // finalize MPI
   MPI_Finalize ();

   printf ("done, with absolute rank %d\n", rank);

   return 0;
}
