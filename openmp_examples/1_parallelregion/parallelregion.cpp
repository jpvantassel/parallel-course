#include "omp.h"                // Include openmp header.
#include "iostream"             // Include iostream to print.

int main(){
std::cout<<"Serial Code Before"
         <<std::endl;           // Will print only once.
#pragma omp parallel            // Begin parallel region, threads forked here.
{
#pragma omp critical
{
std::cout<<"Parallel Code"
         <<std::endl;           // Will print once for each thread.
}
}                               // End of parallel region, threads joined here.
std::cout<<"Serial Code After"
         <<std::endl;           // Will print only once.
}
