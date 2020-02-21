#include "omp.h"                // Include openmp header.
#include "iostream"             // Include iostream to print.

int main(){
#pragma omp parallel            // Begin parallel region, threads forked here.
{
#pragma omp critical            // Begin critical region, single thread at time.
{
std::cout<<"Hello from thread "
         <<omp_get_thread_num() // omp function to get current thread number.
         <<"!"
         <<std::endl;
}
}
}
