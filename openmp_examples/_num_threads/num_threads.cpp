#include <omp.h>
#include <iostream>


int main(){

std::cout<<"Num Threads: "<<omp_get_max_threads()<<std::endl;

#pragma omp parallel num_threads(4)
{
#pragma omp critical
std::cout<<"Thread Number: "<<omp_get_thread_num()<<std::endl;
}

#pragma omp parallel
{
#pragma omp critical
std::cout<<"Thread Number: "<<omp_get_thread_num()<<std::endl;
}


std::cout<<"Num Threads: "<<omp_get_max_threads()<<std::endl;

}
