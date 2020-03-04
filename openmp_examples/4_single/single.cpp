#include "iostream"
#include "omp.h"

int main(){
  
#pragma omp parallel                        // Begin parallel region, threads forked.
{
#pragma omp single                          // Only a single thread enters.
{
std::cout<<omp_get_thread_num()<<std::endl; // Code executed by single thread.
}                                           // All other threads wait here at barrier.
}                                           // End of parallel region, threads joined.

}
