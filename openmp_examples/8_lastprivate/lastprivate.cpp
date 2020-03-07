#include "omp.h"
#include "iostream"

int main(){

const int n=12;                             // Define iterations
int var = 10;                               // Define variable
#pragma omp parallel default(none) shared(var, std::cout)
{

#pragma omp single
std::cout<<"Parallel Region: var = "<<var<<std::endl;

#pragma omp for schedule(dynamic, 1) private(var)
                                            // Should be lastprivate(var)!
for (int i=0; i<n; i++){
  for (int j=0; j<i+1; j++){
    var = 20;                               // Set var to 20
    if (i == n-1 && j==i)                   // If last iteration
      var = 30;                             // Set var to 30
  }
}                                           // End parallel for

#pragma omp single
std::cout<<"Parallel Region: var = "<<var<<std::endl;

}                                           // End parallel region

std::cout<<"Serial Region: var = "<<var<<std::endl;
}
