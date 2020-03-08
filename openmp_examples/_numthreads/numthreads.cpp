#include <iostream>
#include "omp.h"

int main(){

std::cout<<"Serial Region:"<<std::endl;
std::cout<<"  omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"  omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

omp_set_num_threads(8);
std::cout<<"omp_set_num_threads(8)"<<std::endl;
std::cout<<std::endl;

std::cout<<"  Serial Region:"<<std::endl;
std::cout<<"    omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"    omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

#pragma omp parallel
{
#pragma omp single
{
std::cout<<"  Parallel Region:"<<std::endl;
std::cout<<"    omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"    omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

omp_set_num_threads(6);
std::cout<<"omp_set_num_threads(6)"<<std::endl;
std::cout<<std::endl;

std::cout<<"  Parallel Region:"<<std::endl;
std::cout<<"    omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"    omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
}
}

}
