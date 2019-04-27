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

   MPI_Group group0, group01, group23, group12;
   MPI_Comm comm0;
   MPI_Comm comm01;
   MPI_Comm comm23;
   MPI_Comm comm12;
   MPI_Group groupWorld;
   int ranks0[1]  = {0};
   int ranks01[2] = {0, 1};
   int ranks23[2] = {2, 3};
   int ranks12[2] = {1, 2};

   // get the universal group from MPI_COMM_WORLD
   MPI_Comm_group (MPI_COMM_WORLD, &groupWorld);

   // create groups from the universal group
   MPI_Group_incl (groupWorld, 1, ranks0, &group0);
   MPI_Group_incl (groupWorld, 2, ranks01, &group01);
   MPI_Group_incl (groupWorld, 2, ranks23, &group23);
   MPI_Group_incl (groupWorld, 2, ranks12, &group12);

   // show that the first group size = 2 irregardless of the rank
   int size;
   MPI_Group_size (group01, &size);
   printf ("group01 size = %d from rank %d\n", size, rank);

   // get the relative rank number of the first group
   int g01_rank;
   MPI_Group_rank (group01, &g01_rank);

   // get the relative rank number of the second group
   int g23_rank;
   MPI_Group_rank (group23, &g23_rank);

   // get the relative rank number of the third group
   int g12_rank;
   MPI_Group_rank (group12, &g12_rank);
   if (g12_rank == MPI_UNDEFINED) {
      printf ("third group rank is undefined for absolute rank %d\n", rank);
   } else {
      printf ("group rank = %d for absolute rank %d\n", g12_rank, rank);
   }

   int g0_rank;
   MPI_Group_rank (group0, &g0_rank);

   // union groups 1 and 3
   MPI_Group group01u12;
   int g1u3_rank;
   MPI_Group_union (group01, group12, &group01u12);
   MPI_Group_rank (group01u12, &g1u3_rank);
   if (g1u3_rank != MPI_UNDEFINED) printf ("group01u12 contains absolute rank %d via relative rank %d\n", rank, g1u3_rank);

   // union groups 1 and 2
   MPI_Group group01u23;
   int g1u2_rank;
   MPI_Group_union (group01, group23, &group01u23);
   MPI_Group_rank (group01u23, &g1u2_rank);
   if (g1u2_rank != MPI_UNDEFINED) printf ("group01u23 contains absolute rank %d via relative rank %d\n", rank, g1u2_rank);

   // intersect groups 1 and 3
   MPI_Group group01n13;
   int g1n3_rank;
   MPI_Group_intersection (group01, group12, &group01n13);
   MPI_Group_rank (group01n13, &g1n3_rank);
   if (g1n3_rank != MPI_UNDEFINED) printf ("group01n13 contains absolute rank %d via relative rank %d\n", rank, g1n3_rank);

   MPI_Group group0u23;
   int g0u23_rank;
   MPI_Group_union (group0, group23, &group0u23);
   MPI_Group_rank (group0u23, &g0u23_rank);
   if (g0u23_rank != MPI_UNDEFINED) printf ("group0u23 contains absolute rank %d via relative rank %d\n", rank, g0u23_rank);

   // create a communicator for the first group
   MPI_Comm_create (MPI_COMM_WORLD, group01, &comm01);

   MPI_Comm_create (MPI_COMM_WORLD, group0, &comm0);

   // create a communicator for the second group
   MPI_Comm_create (MPI_COMM_WORLD, group23, &comm23);

   // create a communicator for the third group
   MPI_Comm_create (MPI_COMM_WORLD, group12, &comm12);

   // display that this is a null communicator for the other ranks not in group 1
   if (comm01 == MPI_COMM_NULL) {
      printf ("comm01 is a null communicator for rank %d\n", rank);
   }

   // intra-communication within group 3
   float pi = 3.141592f;
   float e  = 2.718281f;
   float buf;
   MPI_Status stat;
   if (g12_rank == 0) MPI_Send (&pi, 1, MPI_FLOAT, 1, EX_TAG, comm12);
   if (g12_rank == 1) {
      MPI_Recv (&buf, 1, MPI_FLOAT, 0, EX_TAG, comm12, &stat);
      printf ("group 3 rank 1 received pi = %f\n", buf);
   }

   // identify this communicator as an intra-communicator
   int test;
   if (g01_rank != MPI_UNDEFINED) MPI_Comm_test_inter (comm01, &test);
   if (g01_rank != MPI_UNDEFINED) if (test == true)  printf ("communicator 1 is an inter-communicator\n");
   if (g01_rank != MPI_UNDEFINED) if (test == false) printf ("communicator 1 is an intra-communicator\n");

   // create intra-communicator for group 1 union 2
   MPI_Comm intraComm01u23;
   MPI_Comm_create (MPI_COMM_WORLD, group01u23, &intraComm01u23);

   MPI_Comm intraComm0u23;
   MPI_Comm_create (MPI_COMM_WORLD, group0u23, &intraComm0u23);

   // verify that the intra-communicator is an intra-communicator
   if (g01_rank != MPI_UNDEFINED) MPI_Comm_test_inter (intraComm01u23, &test);
   if (g01_rank != MPI_UNDEFINED) if (test == true)  printf ("intra-communicator 1 union 2 is an inter-communicator\n");
   if (g01_rank != MPI_UNDEFINED) if (test == false) printf ("intra-communicator 1 union 2 is an intra-communicator\n");

   MPI_Comm interComm01to23;
   MPI_Comm interComm23to01;
   MPI_Comm interComm0to23;
   MPI_Comm interComm23to0;

   // group 1 communicates with group 2
   if (g01_rank != MPI_UNDEFINED) MPI_Intercomm_create (comm01, 0, intraComm01u23, 2, INTERCOMM_TAG, &interComm01to23);
   if (g01_rank != MPI_UNDEFINED) printf ("created inter communicator from world rank %d from communicator containing ranks starting from %d to starting world rank %d at tag %d\n", rank, g01_rank, 2, INTERCOMM_TAG);

   // group 2 communicates with group 1
   if (g23_rank != MPI_UNDEFINED) MPI_Intercomm_create (comm23, 0, intraComm01u23, 0, INTERCOMM_TAG, &interComm23to01);
   if (g23_rank != MPI_UNDEFINED) printf ("created inter communicator from world rank %d from communicator containing ranks starting from %d to starting world rank %d at tag %d\n", rank, g23_rank, 2, INTERCOMM_TAG);

   if (g0_rank  != MPI_UNDEFINED) MPI_Intercomm_create (comm0, 0, intraComm0u23, 2, INTERCOMM_TAG, &interComm0to23);
   if (g23_rank != MPI_UNDEFINED) MPI_Intercomm_create (comm23, 0, intraComm0u23, 2, INTERCOMM_TAG, &interComm23to0);

   // identify this communicator as an inter-communicator
   if (g01_rank != MPI_UNDEFINED) MPI_Comm_test_inter (interComm01to23, &test);
   if (g01_rank != MPI_UNDEFINED) if (test == true)  printf ("communicator 1 to 2 is an inter-communicator\n");
   if (g01_rank != MPI_UNDEFINED) if (test == false) printf ("communicator 1 to 2 is an intra-communicator\n");

   // identify this communicator as an intra-communicator
   if (g23_rank != MPI_UNDEFINED) MPI_Comm_test_inter (interComm23to01, &test);
   if (g23_rank != MPI_UNDEFINED) if (test == true)  printf ("communicator 2 to 1 is an inter-communicator\n");
   if (g23_rank != MPI_UNDEFINED) if (test == false) printf ("communicator 2 to 1 is an intra-communicator\n");

   // send a value from group 1 to group 2 via inter-communication between groups
   buf = 0.0f;
   if (g01_rank == 0) MPI_Send (&e,   1, MPI_FLOAT, 0, INTERCOMM_P2P, interComm01to23);
   if (g23_rank == 0) MPI_Recv (&buf, 1, MPI_FLOAT, 0, INTERCOMM_P2P, interComm23to01, &stat);
   if (g23_rank == 0) printf ("group 2 relative rank 0 received e = %f from group 1 relative rank 0\n", buf);

   // verify that the group associated with the inter communicators are identical to their intra-communicator counterparts
   MPI_Group testGroup;
   if (g01_rank != MPI_UNDEFINED) MPI_Comm_group (interComm01to23, &testGroup);
   if (g01_rank != MPI_UNDEFINED) MPI_Group_compare (group01, testGroup, &test);
   if (g01_rank != MPI_UNDEFINED && test == MPI_IDENT)   printf ("the group for inter communication handle from group 1 to 2 is identical to communicator 1's group\n");
   if (g01_rank != MPI_UNDEFINED && test == MPI_SIMILAR) printf ("the group for inter communication handle from group 1 to 2 is similar to communicator 1's group\n");
   if (g01_rank != MPI_UNDEFINED && test == MPI_UNEQUAL) printf ("the group for inter communication handle from group 1 to 2 is unequal to communicator 1's group\n");

   if (g23_rank != MPI_UNDEFINED) MPI_Comm_group (interComm23to01, &testGroup);
   if (g23_rank != MPI_UNDEFINED) MPI_Group_compare (group23, testGroup, &test);
   if (g23_rank != MPI_UNDEFINED && test == MPI_IDENT)   printf ("the group for inter communication handle from group 2 to 1 is identical to communicator 2's group\n");
   if (g23_rank != MPI_UNDEFINED && test == MPI_SIMILAR) printf ("the group for inter communication handle from group 2 to 1 is similar to communicator 2's group\n");
   if (g23_rank != MPI_UNDEFINED && test == MPI_UNEQUAL) printf ("the group for inter communication handle from group 2 to 1 is unequal to communicator 2's group\n");

   // free the groups
   MPI_Group_free (&group01);
   MPI_Group_free (&group23);
   MPI_Group_free (&group12);
   MPI_Group_free (&group01u12);
   MPI_Group_free (&group01n13);
   MPI_Group_free (&testGroup);

   // free the communicator handles
   if (g01_rank != MPI_UNDEFINED) {
      MPI_Comm_free (&comm01);
      MPI_Comm_free (&interComm01to23);
   }
   if (g23_rank != MPI_UNDEFINED) {
      MPI_Comm_free (&comm23);
      MPI_Comm_free (&interComm23to01);
   }

   if (g12_rank != MPI_UNDEFINED) {
      MPI_Comm_free (&comm12);
   }

   // finalize MPI
   MPI_Finalize ();

   printf ("done, with absolute rank %d\n", rank);

   return 0;
}
