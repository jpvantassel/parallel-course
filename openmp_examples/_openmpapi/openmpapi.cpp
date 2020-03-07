#include <iostream>
#include "omp.h"

int main(){

std::cout<<"Serial Region"<<std::endl;

std::cout<<"  omp_get_thread_num()  "<<omp_get_thread_num()<<std::endl;
std::cout<<"  omp_get_num_procs()   "<<omp_get_num_procs()<<std::endl;
std::cout<<"  omp_get_num_threads() "<<omp_get_num_threads()<<std::endl;

std::cout<<std::endl<<"Parallel Region"<<std::endl;
#pragma omp parallel
{

#pragma omp critical
{
std::cout<<"  omp_get_thread_num()  "<<omp_get_thread_num()<<std::endl;
std::cout<<"  omp_get_num_procs()   "<<omp_get_num_procs()<<std::endl;
std::cout<<"  omp_get_num_threads() "<<omp_get_num_threads()<<std::endl;
}
}


}
