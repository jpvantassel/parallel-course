# Work Scheduling

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

In the previous examples we were performing parallel work, but we did not
concern ourselves with how the work was divided between threads. In this
section we will learn how to dictate to `omp` how to divide the work.

Work can be divided in one of three ways:

- `static`: All threads receive equal work. Fast and simple.
- `dynamic`: Allows the scheduler free-reign on how to divide the work between
threads. Good load balancing, but costs more overhead than `static`.
- `guided`: The scheduler begins with a large chunk size, and shrink it until
it reaches the user-defined minimum. `guided` may in certain cases offer better
performance than `dynamic`. Set the minimum chunk size such that the
parallelization is still performant.

The schedule types listed above each come with a `chunk size` argument, they
are detailed as follows.

- `schedule(static, N)`: Each thread receives N iterations starting from thread
0 and increasing to N-1 threads. If there is more work it begins again from 0.
If N is not specified then the work is divided as equally as possible in terms
of loop iterations (e.g., if there are 100 iterations and 4 threads each thread
would receive 25).
- `schedule(dynamic, N)`: Each thread receives N iterations however the thread
order is decided at runtime. If N is not specified a default `chunk size` is
assumed this may be 1.
- `schedule(guided, N)`: `omp` decides how much work should go to each thread.
As work is assigned, `omp` decides whether to keep the work for the next thread
the same as the previous or to decrease it (the workload cannot be increased).
The `chunk size` in this case is the minimum `chunk size`, if not defined it
defaults to 1.
- `schedule(runtime)`: In this case the schedule type is set by the environment
variable `OMP_SCHEDULE`, which is useful to see what type of scheduling is the
most performant. For example to set `runtime` to `guided,8` use
`export OMP_SCHEDULE='guided,8'`.

Lets compare the three different types of scheduling.

> scheduling.cpp

```cpp
#include <iostream>
#include "omp.h"

int main(){

#pragma omp parallel
{

#pragma omp single
std::cout<<"schedule(static,2)"<<std::endl;
#pragma omp for schedule(static, 2)
for (int i=0; i<12; i++){
  #pragma omp critical
  std::cout<<"  Thread Number: "<<omp_get_thread_num()<<" i="<<i<<std::endl;
}

#pragma omp single
std::cout<<"schedule(dynamic,2)"<<std::endl;
#pragma omp for schedule(dynamic, 2)
for (int i=0; i<12; i++){
  #pragma omp critical
  std::cout<<"  Thread Number: "<<omp_get_thread_num()<<" i="<<i<<std::endl;
}

#pragma omp single
std::cout<<"schedule(guided, 2)"<<std::endl;
#pragma omp for schedule(guided, 2)
for (int i=0; i<12; i++){
  #pragma omp critical
  std::cout<<"  Thread Number: "<<omp_get_thread_num()<<" i="<<i<<std::endl;
}

}
}
```

> ./a.out

```bash
schedule(static,2)
  Thread Number: 0 i=0
  Thread Number: 0 i=1
  Thread Number: 1 i=2
  Thread Number: 1 i=3
  Thread Number: 2 i=4
  Thread Number: 2 i=5
  Thread Number: 3 i=6
  Thread Number: 3 i=7
  Thread Number: 0 i=8
  Thread Number: 0 i=9
  Thread Number: 1 i=10
  Thread Number: 1 i=11
schedule(dynamic,2)
  Thread Number: 0 i=0
  Thread Number: 0 i=1
  Thread Number: 1 i=2
  Thread Number: 1 i=3
  Thread Number: 2 i=4
  Thread Number: 2 i=5
  Thread Number: 3 i=6
  Thread Number: 3 i=7
  Thread Number: 0 i=8
  Thread Number: 0 i=9
  Thread Number: 1 i=10
  Thread Number: 1 i=11
schedule(guided, 2)
  Thread Number: 1 i=0
  Thread Number: 1 i=1
  Thread Number: 1 i=2
  Thread Number: 2 i=3
  Thread Number: 2 i=4
  Thread Number: 2 i=5
  Thread Number: 0 i=6
  Thread Number: 0 i=7
  Thread Number: 3 i=8
  Thread Number: 3 i=9
  Thread Number: 1 i=10
  Thread Number: 1 i=11
```

Now lets compare performance between the three schedule types, when we want to
initialize a triangular matrix (i.e., an unbalanced load condition).

> compareschedule.cpp

```cpp
#include <iostream>
#include <chrono>
#include "omp.h"

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
```

> ./a.out

```bash
Elapsed Time Static 0.398055 sec.
Elapsed Time Dynamic 0.0430165 sec.
Elapsed Time Guided 0.0428927 sec.
```

So for this unbalanced load case we see that the `Dynamic` and `Guided`
scheduling performs significantly better than the `Static`.
