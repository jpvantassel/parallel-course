#include <iostream>
#include <omp.h>

int main(){

#pragma omp parallel
{

#pragma omp single
std::cout<<"schedule(static,2)"<<std::endl;
#pragma omp for schedule(static, 2)
for (int i=0; i<12; i++){
  #pragma omp critical
  std::cout<<"  Thread Number: "<<omp_get_thread_num()<<" i="<<i<<std::endl;
}

#pragma omp single
std::cout<<"schedule(dynamic,2)"<<std::endl;
#pragma omp for schedule(dynamic, 2)
for (int i=0; i<12; i++){
  #pragma omp critical
  std::cout<<"  Thread Number: "<<omp_get_thread_num()<<" i="<<i<<std::endl;
}

#pragma omp single
std::cout<<"schedule(guided, 2)"<<std::endl;
#pragma omp for schedule(guided, 2)
for (int i=0; i<12; i++){
  #pragma omp critical
  std::cout<<"  Thread Number: "<<omp_get_thread_num()<<" i="<<i<<std::endl;
}

}
}
