#include "iostream"
#include "omp.h"

int main(){

const int n = 80;                             // Make psum=10 (b/c 8 threads)
int psum=5;                                   // Add initial value so psum=15

#pragma omp parallel firstprivate(psum)            // Make psum private, MISTAKE!
{

#pragma omp for
for (int i=0; i<n; i++){
  psum += 1;                                  // Count to n using seperate threads
}

#pragma omp for
for (int i=0; i<omp_get_num_threads(); i++){

#pragma omp critical                          // Use critical to avoid race condition
std::cout<<psum<<", ";                        // Print partial sums from each thread

}

}

}
