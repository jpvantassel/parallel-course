#include "omp.h"
#include "iostream"

int main(){

const int n=5;                              // Define iterations
int var=24;                                 // Define variable

std::cout<<"Serial Region - Variable var="<<var<<std::endl;

#pragma omp parallel default(none) shared(var, std::cout)
{

#pragma omp single
std::cout<<"Parallel Region - Variable var="<<var<<std::endl;

#pragma omp for schedule(dynamic, 3) lastprivate(var)
for (int i=0; i<n; i++){
  var = i;
  #pragma omp critical
  {
  std::cout<<"Thread Number: "<<omp_get_thread_num()<<" - "
           <<"Variable var="<<var<<std::endl;
  }
}                                           // End parallel for

#pragma omp single
std::cout<<"Parallel Region - Variable var="<<var<<std::endl;

}                                           // End parallel region

std::cout<<"Serial Region - Variable var = "<<var<<std::endl;

}
