# The Parallel Region

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

## Basic Syntax

- A parallel regions begins with `#pragma omp parallel`.
- At the start of the parallel region the threads are forked.
- At the end of the parallel region is an implied barrier.
- Once all threads reach this implied barrier the threads are merged.

A simple example would be:

> parallelregion.cpp

```cpp
#include <iostream>             // Include iostream for printing.
#include "omp.h"                // Include openmp header.

int main(){
std::cout<<"Serial Code Before"
         <<std::endl;           // Will print only once.
#pragma omp parallel            // Begin parallel region, threads forked here.
{
#pragma omp critical
{
std::cout<<"Parallel Code"
         <<std::endl;           // Will print once for each thread.
}
}                               // End of parallel region, threads joined here.
std::cout<<"Serial Code After"
         <<std::endl;           // Will print only once.
}
```

The output where the environment variable `OMP_NUM_THREADS` has been set equal
to 4 would be:

> ./a.out

```bash
Serial Code Before
Parallel Code
Parallel Code
Parallel Code
Parallel Code
Serial Code After
```

## Api Basics

The `omp` API (Application Programming Interface) includes a number of helpful
built in functions. You have seen a few of these previously for example
`omp_get_thread_num()` in the above `helloworld.cpp` example. A thorough
introduction to the basic API calls are presented here. It is important to
take some time now to understand how these calls behave when inside and outside
the `parallel region` (as it may be different) in order to understand the
remaining examples in this module.

The below example includes the basic API calls with their results shown below.

> mpiapi.cpp

```cpp
#include <iostream>
#include "omp.h"

int main(){

std::cout<<"Serial Region"<<std::endl;

std::cout<<"  omp_in_parallel()     "<<omp_in_parallel()<<std::endl;
std::cout<<"  omp_get_max_threads() "<<omp_get_max_threads()<<std::endl;
std::cout<<"  omp_get_thread_num()  "<<omp_get_thread_num()<<std::endl;
std::cout<<"  omp_get_num_procs()   "<<omp_get_num_procs()<<std::endl;
std::cout<<"  omp_get_num_threads() "<<omp_get_num_threads()<<std::endl;

std::cout<<std::endl<<"Parallel Region"<<std::endl;
#pragma omp parallel
{

#pragma omp critical
{
std::cout<<"  omp_in_parallel()     "<<omp_in_parallel()<<std::endl;
std::cout<<"  omp_get_max_threads() "<<omp_get_max_threads()<<std::endl;
std::cout<<"  omp_get_thread_num()  "<<omp_get_thread_num()<<std::endl;
std::cout<<"  omp_get_num_procs()   "<<omp_get_num_procs()<<std::endl;
std::cout<<"  omp_get_num_threads() "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;
}

#pragma omp single
std::cout<<"Worksharing Region"<<std::endl;

#pragma omp for
for (int i=0; i<4; i++){
  #pragma omp critical
  {
  std::cout<<"  omp_in_parallel()     "<<omp_in_parallel()<<std::endl;
  std::cout<<"  omp_get_max_threads() "<<omp_get_max_threads()<<std::endl;
  std::cout<<"  omp_get_thread_num()  "<<omp_get_thread_num()<<std::endl;
  std::cout<<"  omp_get_num_procs()   "<<omp_get_num_procs()<<std::endl;
  std::cout<<"  omp_get_num_threads() "<<omp_get_num_threads()<<std::endl;
  std::cout<<std::endl;
  }
}

}

}
```

> ./a.out

```bash
Serial Region
  omp_in_parallel()     0
  omp_get_max_threads() 4
  omp_get_thread_num()  0
  omp_get_num_procs()   8
  omp_get_num_threads() 1

Parallel Region
  omp_in_parallel()     1
  omp_get_max_threads() 4
  omp_get_thread_num()  1
  omp_get_num_procs()   8
  omp_get_num_threads() 4

Worksharing Region
  omp_in_parallel()     1
  omp_get_max_threads() 4
  omp_get_thread_num()  0
  omp_get_num_procs()   8
  omp_get_num_threads() 4

  omp_in_parallel()     1
  omp_get_max_threads() 4
  omp_get_thread_num()  2
  omp_get_num_procs()   8
  omp_get_num_threads() 4

  omp_in_parallel()     1
  omp_get_max_threads() 4
  omp_get_thread_num()  3
  omp_get_num_procs()   8
  omp_get_num_threads() 4

  omp_in_parallel()     1
  omp_get_max_threads() 4
  omp_get_thread_num()  2
  omp_get_num_procs()   8
  omp_get_num_threads() 4

  omp_in_parallel()     1
  omp_get_max_threads() 4
  omp_get_thread_num()  1
  omp_get_num_procs()   8
  omp_get_num_threads() 4

  omp_in_parallel()     1
  omp_get_max_threads() 4
  omp_get_thread_num()  3
  omp_get_num_procs()   8
  omp_get_num_threads() 4

  omp_in_parallel()     1
  omp_get_max_threads() 4
  omp_get_thread_num()  0
  omp_get_num_procs()   8
  omp_get_num_threads() 4
```

And another example that examines the difference between
`omp_get_max_threads()` and `omp_get_num_threads()`.

> numthreads.cpp

```cpp
#include <iostream>
#include "omp.h"

int main(){

std::cout<<"Serial Region:"<<std::endl;
std::cout<<"  omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"  omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

omp_set_num_threads(8);
std::cout<<"  omp_set_num_threads(8)"<<std::endl;
std::cout<<std::endl;

std::cout<<"  omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"  omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

#pragma omp parallel
{
#pragma omp single
{
std::cout<<"Parallel Region:"<<std::endl;
std::cout<<"  omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"  omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

omp_set_num_threads(6);
std::cout<<"  omp_set_num_threads(6)"<<std::endl;
std::cout<<std::endl;

std::cout<<"  omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"  omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
}
}

}
```

> ./a.out

```bash
Serial Region:
  omp_get_max_threads()  4
  omp_get_num_threads()  1

  omp_set_num_threads(8)

  omp_get_max_threads()  8
  omp_get_num_threads()  1

Parallel Region:
  omp_get_max_threads()  8
  omp_get_num_threads()  8

  omp_set_num_threads(6)

  omp_get_max_threads()  6
  omp_get_num_threads()  8
```

In summary

- `omp_in_parallel()`: 0 if in serial region, 1 otherwise (parallel region and
worksharing).
- `omp_get_num_procs()`: Always the number of processors (i.e., maximum number
of hardware threads).
- `omp_get_num_threads()`: Number of currently spawned threads, 1 if in serial
region.
- `omp_get_thread_num()`: 0 if in serial region, 0 to N-1 otherwise. Where N is
the number of available threads (i.e., equal to `omp_get_num_threads`).
- `omp_get_max_threads()`: Depends on the following listed in order of highest
to lowest precidence `omp_set_num_threads(#)`>`OMP_NUM_THREADS`>
`omp_get_num_procs()`.
  - The default is the number of hardware threads available (i.e., equal to
  `omp_get_num_procs()`).
  - If the environment variable `OMP_NUM_THREADS` has been set then it is replace
  with that value.
  - If the `omp_set_num_threads(#)` api call has been made then it returns `#`,
  ignoring the value from `OMP_NUM_THREADS` if it has been set.
