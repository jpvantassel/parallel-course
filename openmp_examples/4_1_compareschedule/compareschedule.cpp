#include "omp.h"
#include "iostream"
#include "chrono"

int main(){

const int n=10000;                  // Define triangular array 10000 x 10000.
int **array = new int*[n];
for (int i=0; i<n; i++){            // Setup the triangular nature.
  array[i] = new int [i+1];
}

// Static
auto start_static = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(static)
for (int i=0; i<n; ++i){
  for (int j=0; j<i+1; j++){
    array[i][j] = 0;
  }
}
auto end_static = std::chrono::high_resolution_clock::now();
double elapsed_time_static = std::chrono::duration_cast<std::chrono::nanoseconds>(end_static-start_static).count();
elapsed_time_static *= 1e-9;
std::cout<<"Elapsed Time Static "<<elapsed_time_static<<" sec."<<std::endl;

// Dynamic
auto start_dynamic = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(dynamic)
for (int i=0; i<n; ++i){
  for (int j=0; j<i+1; j++){
    array[i][j] = 1;
  }
}
auto end_dynamic = std::chrono::high_resolution_clock::now();
double elapsed_time_dynamic = std::chrono::duration_cast<std::chrono::nanoseconds>(end_dynamic-start_dynamic).count();
elapsed_time_dynamic *= 1e-9;
std::cout<<"Elapsed Time Dynamic "<<elapsed_time_dynamic<<" sec."<<std::endl;

// Guided
auto start_guided = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(guided, 1)
for (int i=0; i<n; ++i){
  for (int j=0; j<i+1; j++){
    array[i][j] = 2;
  }
}
auto end_guided = std::chrono::high_resolution_clock::now();
double elapsed_time_guided = std::chrono::duration_cast<std::chrono::nanoseconds>(end_guided-start_guided).count();
elapsed_time_guided *= 1e-9;
std::cout<<"Elapsed Time Guided "<<elapsed_time_guided<<" sec."<<std::endl;

for (int i=0; i<n; ++i){
  delete[]array[i];
}
delete[]array;
}
