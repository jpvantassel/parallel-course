#include "omp.h"
#include "iostream"
#include "chrono"

int main(){

const int nrows=10000;                  // Define triangular array 10000 x 10000.
int **rows = new int*[nrows];
int **tmps = new int*[nrows];

for (int i=0; i<nrows; ++i){            // Setup the triangular nature.
  rows[i] = new int [i];
  tmps[i] = new int [i];
}

// Static
auto start_static = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(static)
for (int i=0; i<nrows; ++i){
  for (int j=0; j<i; ++j){
    tmps[i][j] = rows[i][j]*rows[i][j];
  }
}
auto end_static = std::chrono::high_resolution_clock::now();
double elapsed_time_static = std::chrono::duration_cast<std::chrono::nanoseconds>(end_static-start_static).count();
elapsed_time_static *= 1e-9;
std::cout<<"Elapsed Time Static "<<elapsed_time_static<<" sec."<<std::endl;

// Dynamic
auto start_dynamic = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(dynamic)
for (int i=0; i<nrows; ++i){
  for (int j=0; j<i; ++j){
    tmps[i][j] = rows[i][j]*rows[i][j];
  }
}
auto end_dynamic = std::chrono::high_resolution_clock::now();
double elapsed_time_dynamic = std::chrono::duration_cast<std::chrono::nanoseconds>(end_dynamic-start_dynamic).count();
elapsed_time_dynamic *= 1e-9;
std::cout<<"Elapsed Time Dynamic "<<elapsed_time_dynamic<<" sec."<<std::endl;

// Guided
auto start_guided = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(guided, 1)
for (int i=0; i<nrows; ++i){
  for (int j=0; j<i; ++j){
    tmps[i][j] = rows[i][j]*rows[i][j];
  }
}
auto end_guided = std::chrono::high_resolution_clock::now();
double elapsed_time_guided = std::chrono::duration_cast<std::chrono::nanoseconds>(end_guided-start_guided).count();
elapsed_time_guided *= 1e-9;
std::cout<<"Elapsed Time Guided "<<elapsed_time_guided<<" sec."<<std::endl;

for (int i=0; i<nrows; ++i){            // Deallocate memory.
  delete[]tmps[i];
  delete[]rows[i];
}
delete[]tmps;
delete[]rows;
}
