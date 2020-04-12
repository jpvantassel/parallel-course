# Work Sharing

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

## The Basics

After the `fork` and `join` model the next most significant concept to
understand is `work sharing`. Without worksharing we cannot
utilize the threads we `fork`, and can only perform `replicated work`.

We saw an example of `replicated work` in the previous example
`parallelregion.cpp` where we printed "Parallel Code" `N` times where `N` was
the number of threads. We will generally not want to perform `replicated work`
(because for most problems it will do as much work as serial code but take
longer than serial code because of the additional overhead of forking and
joining thread). Instead we will want to use `work sharing` to improve our
code's performance.

All worksharing constructs in `omp`:

1. Are intended to prevent `replicated work`, and
2. End with an implied barrier.

## Parallel For

One of the most common `work sharing` constructs is `parallel for` which
allows for-loops to run in parallel (more details on this to follow). For
now lets consider a simple `for` loop which can be parallelized.

> for.cpp

```cpp
#include <iostream>
#include "omp.h"

int main(){

const int size=15;
auto evens = new int [size];    // Instantiate a new array.

#pragma omp parallel for        // Begin parallel region and parallel for.
for (int i=0; i<size; ++i){     // Loop across the array (in parallel).
  evens[i] = 2+i*2;
}

for (int i=0; i<size; ++i){
  std::cout<<evens[i]<<", ";    // Print each entry of the array (in serial).
}
std::cout<<std::endl;

delete evens;                   // Clean up array.
}
```

> ./a.out

```bash
2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
```

_When using `parallel for` you cannot use `break` statements as you will receive
a compiler error, but you can use `continue` to achieve similar results._

## Single

The next most widely used `worksharing` construct is `single`. As the name
implies, code within a `single` worksharing region is only executed by one and
only one thread. Since `single` is a worksharing construct the other threads
must wait at the `single` region's barrier located at the end of its scope.

> single.cpp

```cpp
#include "omp.h"
#include <iostream>

int main(){

#pragma omp parallel                        // Begin parallel region, threads forked.
{
#pragma omp single                          // Only a single thread enters.
{
std::cout<<omp_get_thread_num()<<std::endl; // Code executed by single thread.
}                                           // All other threads wait here at barrier.
}                                           // End of parallel region, threads joined.

}
```

When I executed this program for the first time, it returned the following.

> ./a.out

```bash
7                                           # Thread number between 0 and N-1
```

_Since my machine has 8 hardware threads (and I have not defined the
environment variable `OMP_NUM_THREADS`), re-running `a.out` can return any value
between 0 and N-1. But it will only ever return a single value per run because
of our use of the `single` construct._

## Orphaned Worksharing

`omp` allows the definition of a `parallel region` to be separated from the
corresponding `worksharing construct`. This allows the programmer to build
general functions/classes that can be executed in serial or parallel.

To illustrate, a simple table is shown below to illustrate the interactions
between the `parallel region` and `worksharing construct`.

|                   | Parallel - Yes    | Parallel - No          |
|:-----------------:|:-----------------:|:----------------------:|
| Worksharing - Yes | Parallel          | Pragma ignored, Serial |
| Worksharing - No  | Replicated Work   | Serial                 |

Any `worksharing construct` defined outside of a `parallel regions` lexical
scope (i.e., `{}` in C/C++) is called `orphaned worksharing`. This could be
inside of a function that can be called inside or outside the parallel region.
If the function is called in a `serial region` then the pragma is ignored the
code is executed serially, otherwise the code is executed in parallel.
