#include <iostream>                             // Include for printing
#include "mpi.h"                                // Include to use MPI functions

int main(){
int ierr, nranks, rank;                         // Allocate variables
ierr = MPI_Init(NULL, NULL);                    // Start MPI here

ierr = MPI_Comm_size(MPI_COMM_WORLD, &nranks);  // Number of processes
ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);    // Rank of current process
std::cout<<"Hello from process rank "
         <<rank<<" of "<<nranks<<std::endl;     // Have each process say hello

ierr = MPI_Finalize();                          // End MPI here
}
