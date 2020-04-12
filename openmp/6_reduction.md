# Reduction

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

To make the programmer's life easier, `omp` allows for a common operation
referred to as a reduction to be done almost automatically without any
complicated use of private and shared variables. Below is an example of where
we calculate the sum of numbers stored in an array in parallel.

> reduction.cpp

```cpp
#include <iostream>
#include "omp.h"

int main(){

const int n=10;
int *array{ new int[n]{4,5,1,3,7,8,0,4,1,2} };
int sum = 1;

std::cout<<"Serial Region - sum="<<sum<<std::endl;

#pragma omp parallel default(none) shared(array, std::cout) reduction(+:sum)
{

#pragma omp for
for (int i=0; i<n; i++){
  sum += array[i];
}

#pragma omp critical
std::cout<<"Parallel Region - Thread Number "<<omp_get_thread_num()<<" sum="<<sum<<std::endl;

}

std::cout<<"Serial Region - sum="<<sum<<std::endl;

}
```

When we run this example with `OMP_NUM_THREADS=5` we see.

>./a.out

```bash
Serial Region - sum=1
Parallel Region - Thread Number 1 sum=4
Parallel Region - Thread Number 4 sum=3
Parallel Region - Thread Number 2 sum=15
Parallel Region - Thread Number 0 sum=9
Parallel Region - Thread Number 3 sum=4
Serial Region - sum=36
```

We see that the initial value of `sum` is correctly equal to 1, the partial sums
in the `parallel region` correctly equal 35 (i.e., the sum of `array`), and that
the final value of `sum` is equal to 36 (the initial value 1 + the sum of
`array` which was equal to 35).
