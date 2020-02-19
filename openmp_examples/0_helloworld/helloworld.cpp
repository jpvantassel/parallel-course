#include <iostream>
#include "omp.h"

int main(){

#pragma omp parallel
{
#pragma omp critical
{
std::cout<<"Hello from thread "<<omp_get_thread_num()<<"!"<<std::endl;
}
}
}
