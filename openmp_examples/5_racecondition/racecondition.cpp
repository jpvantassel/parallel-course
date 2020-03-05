#include "iostream"
#include "omp.h"

int main(){

const int n=10;
auto a = new int[n];        // Create an integer array  

a[0] = 2;                   // Initialize the first value
#pragma omp parallel for    // Begin parallel region and work sharing
for (int i=1; i<n; i++){    // Loop across array
  a[i] = a[i-1]*2;          // Race Condition!
}

for (int i=0; i<n; i++){    // Loop across array
  std::cout<<a[i]<<", ";    // Print each value
}
std::cout<<std::endl;

delete[] a;                 // Clean up dynamic array
}
