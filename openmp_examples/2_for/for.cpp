#include "omp.h"                // Include openmp header.
#include "iostream"             // Include iostream to print.

int main(){

const int size=15;
auto evens = new int [size];    // Instantiate a new array.

#pragma omp parallel for        // Begin parallel region and for worksharing
for (int i=0; i<size; ++i){     // Loop across the array (in parallel).
  evens[i] = 2+i*2;
}

for (int i=0; i<size; ++i){
  std::cout<<evens[i]<<", ";    // Print each entry of the array.
}
std::cout<<std::endl;

delete[] evens;                   // Clean up array.
}
