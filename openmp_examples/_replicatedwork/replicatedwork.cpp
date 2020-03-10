#include <iostream>
#include <omp.h>

int main(){

const int N=4;
const int M=4;

int **array = new int*[M];
for (int i=0; i<M; i++){
  array[i] = new int[N];
}

int i, j;

#pragma omp parallel private(j) num_threads(N)
{
for (j=0; j<N; j++){
  #pragma omp for
  for(i=0; i<M; i++){
    array[i][j] = array[i][j] + j;
    #pragma omp critical
    std::cout<<"Thread Number: "<<omp_get_thread_num()<<" i="<<i<<" j="<<j<<std::endl;
  } 
}
}

for (int j=0; j<N; j++){
  for (int i=0; i<M; i++){
  std::cout<<array[i][j];
  }
  std::cout<<std::endl;
}

for (int i=0; i<M; i++){
  delete[]array[i];
}
delete[]array;

}
