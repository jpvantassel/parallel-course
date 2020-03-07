#include <iostream>
#include "omp.h"

int main(){

const int n=10;
int *array{ new int[n]{4,5,1,3,7,8,0,4,1,2} };
int sum = 1;

std::cout<<"Serial Region - sum="<<sum<<std::endl;

#pragma omp parallel default(none) shared(array, std::cout) reduction(+:sum)
{

#pragma omp for
for (int i=0; i<n; i++){
  sum += array[i];
}

#pragma omp critical
std::cout<<"Parallel Region - Thread Number "<<omp_get_thread_num()<<" sum="<<sum<<std::endl;

}

std::cout<<"Serial Region - sum="<<sum<<std::endl;

}
