#include "iostream"
#include "omp.h"
#include "cmath"

int main(){

const int n=10;
auto a = new int[n];        // Create an integer array  

#pragma omp parallel for    // Begin parallel region and work sharing
for (int i=0; i<n; i++){    // Loop across array
  a[i] = std::pow(2, i)*2;  // Remove Race Condition!
}

for (int i=0; i<n; i++){    // Loop across array
  std::cout<<a[i]<<", ";    // Print each value
}
std::cout<<std::endl;

delete[] a;                 // Clean up dynamic array
}
