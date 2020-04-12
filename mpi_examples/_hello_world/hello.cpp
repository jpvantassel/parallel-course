#include <iostream>
#include <mpi.h>

int main(){

int ierr, nranks, rank;
ierr = MPI_Init(NULL, NULL);
ierr = MPI_Comm_size(MPI_COMM_WORLD, &nranks);
ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
std::cout<<"Hello from process rank "<<rank<<" of "<<nranks<<std::endl;

ierr = MPI_Finalize();
}
