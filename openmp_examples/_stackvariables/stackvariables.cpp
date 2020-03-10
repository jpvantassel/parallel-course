#include <iostream>
#include <omp.h>
#include <thread>
#include <chrono>

int i;
int a;

void slow_func(int i){
  std::chrono::milliseconds timespan(i);
  std::this_thread::sleep_for(timespan);
}


int main(){

#pragma omp parallel for private(i, a)
for (i=0; i<16; i++){
  i = i + 5;
  slow_func(i);
  i = i - 5;
  a = i;
  slow_func(i);
  #pragma omp critical
  std::cout<<"Thread Number: "<<omp_get_thread_num()<<"  i="<<i<<" a="<<a<<std::endl;
}

}
