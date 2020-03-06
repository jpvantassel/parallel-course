#include "omp.h"
#include "iostream"
#include "thread"
#include "chrono"

void slow_func(){                           // Define a slow function.
  std::chrono::milliseconds timespan(5);
  std::this_thread::sleep_for(timespan);
}

int main(){

const int n = 10;                           // Declare arrays
int *evens = new int [n];
int *odds = new int [n];

int even;                                   // Declare even and odd
int odd;

#pragma omp parallel default(none) private(even, odd) shared(evens, odds)
{
#pragma omp for
for (int i=0; i<n; i++){  
  even = 2*i+2;                             // ith even number to even
  slow_func();                              // Seperate threads using slow_func
  odd =  even-1;                            // ith odd number to odd
  evens[i] = even;                          // Assign even and odd
  odds[i] = odd;
}
}

for (int i=0; i<n; i++){                    // Print odds
  std::cout<<odds[i]<<",";
}
std::cout<<std::endl;
for (int i=0; i<n; i++){                    // Print evenss
  std::cout<<evens[i]<<",";
}
std::cout<<std::endl;

delete[]evens;                              // Clean up the arrays
delete[]odds;
}