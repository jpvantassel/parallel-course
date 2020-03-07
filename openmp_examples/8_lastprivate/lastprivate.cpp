#include "omp.h"
#include "iostream"

int main(){

const int n=20;                            // Define triangular matrix nxn
int** tri_mat = new int*[n];
for (int i=0; i<n; i++){
  tri_mat[i] = new int[i+1];
}

int rcount = 0;                            // Define counter variable
#pragma omp parallel default(none) shared(rcount, tri_mat)
                                           // Make counter variable shared
{
#pragma omp for schedule(dynamic) private(rcount)
                                           // Counter variable here is WRONG!
for (int i=0; i<n; i++){
  rcount = 0;                              // Rest counter on each row
  for (int j=0; j<i+1; j++){
    tri_mat[i][j] = 0;  
    rcount += 1;                           // Count elements in row
  }
}                                          // End parallel for (rcount back to 0)
}                                          // End parallel region
std::cout<<"rcount = "<<rcount<<std::endl; // Return counter

for (int i=0; i<n; i++){                   // Clean-up the triangular array
  delete[]tri_mat[i];
}
delete[]tri_mat;
}
