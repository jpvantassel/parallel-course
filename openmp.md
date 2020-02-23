# OpenMP

> Joseph P. Vantassel, The University of Texas at Austin

## Basic Concepts

- __Parallel Region__: Area in which multiple threads may exist.
  - At the begginning threads are `forked`, and
  - At the end `threads` are merged.
- __Work Sharing__: The idea that some tasks can be broken up and performed at
the same time. Without work sharing `forked` threads perform `replicated work`.
- __Public/Private Data__: Threads may share some data in common (i.e.,
`Public Data`) or may keep their own private copy (i.e., `Private Data`).

## Compiling an OpenMP Program

- Include header with `#include "omp.h"`
- Compile with openmp flag
  - _gcc_:`gcc foo.c -fopenmp`
  - _g++_:`g++ foo.cpp -fopenmp`
  - _intel_: `icc foo.c -openmp`

_Note: When using the openmp compiler flag, the variable `_OPENMP` is defined,
and available for compiler directives if necessary._

TODO(jpv): What values does _OPENMP take on.

## Running an OpenMP Program

- Launch a program in the standard way `./a.out`.
- However, the program is senstivate to environment variables including:
  - `OMP_NUM_THREADS`: Sets number of threads that may used.

TODO (jpv): Add other relavant environment variables.

## A Simple OpenMP Program

Below is a simple "Hello World" type program for `openmp`.

```cpp
#include "omp.h"                // Include openmp header.
#include "iostream"             // Include iostream to print.

int main(){
#pragma omp parallel            // Begin parallel region, threads forked here.
{
#pragma omp critical            // Begin critical region, single thread at time.
{
std::cout<<"Hello from thread "
         <<omp_get_thread_num() // omp function to get current thread number.
         <<"!"
         <<std::endl;
}
}
}
```

- To compile `g++ helloworld.cpp -fopenmp`.
- To run `.\a.out`.

If you compiled and ran it correclty you should see something like this ...

```terminal
Hello from thread 4!
Hello from thread 3!
Hello from thread 2!
Hello from thread 7!
Hello from thread 6!
Hello from thread 0!
Hello from thread 5!
Hello from thread 1!
```

_Note: The number of threads you see will depend on the number of hardware
threads your machine supports by default. The machine I used for this test
supports by default 8, therefore I see 8 hellos. Remember you can change that
default by setting the enviornment variable `OMP_NUM_THREADS`
(e.g., `export OMP_NUM_THREADS=4`)_

## Basic Syntax

### Anatomy of an `omp` pragma

Now that we have seen an example let's disect the basic syntax.

There are three parts of an `omp` pragma (also called compiler directive):

- The sentinal: `#pragma omp` which notifies the compiler "Hey `omp` stuff
here!"
- The construct: (e.g. `parallel`) which determines the envoked directive.
- The clause: (e.g. `for`) which contains more specific information to comtrol
how the `construct` behaves.

The three of these together will generally appear together on a single line
`#pragma omp parallel for` which defines the creation of a parallel region with
four threads applied to a `for` loop.

But more details on this later. The important idea here is to understand the
components of an `omp` pragma as more details on the syntax will
be provided later.

### The parallel region

- A parallel regions begins with `#pragma omp parallel`.
- At the start of the parallel region the threads are forked.
- At the end of the parallel region is an implied barrier.
- Once all threads reach this implied barrier the threads are merged.

A simple example would be:

```cpp
#include "omp.h"                // Include openmp header.
#include "iostream"             // Include iostream to print.

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

```terminal
Serial Code Before
Parallel Code
Parallel Code
Parallel Code
Parallel Code
Serial Code After
```

### Work Sharing

After the `fork` and `join` model the next most significant concept to
understand is `work sharing`. Without worksharing we cannot
utilize the threads we `fork` at the beginning of a parallel region, and can
only perform `replicated work`.

We saw an example of `replicated work` in the previous example where we printed
"Parallel Code" `N` times where `N` was the number of threads. We will
generally not want to perform `replicated work` (because for most problems it
will do as much work as serial code but in more time because of the added
overhead). Instead we want to use `work sharing` to increase our code's
performance.

One of the most common `work sharing` constructs is `parallel for` which
allows some for-loops to run in parallel (more details on this to follow). For
now lets consider a simple `for` loop which can be parallelized.

```cpp
#include "omp.h"                // Include openmp header.
#include "iostream"             // Include iostream to print.

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

Result

```bash
2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
```

_Note: When using `parallel for` you cannot use `break` statements
(compiler error), but you can use `continue` to achieve similar results._

### Work Schedule

In the previous examples we were performing parallel work, but we did not
concern ourselves with how the work was divided between threads. In this
section we will learn how to dictate to `omp` how to divide the work.

Work can be divided in one of three ways:

- `static`: All threads receive equal work. Fast and simple.
- `dynamic`: Allows the scheduler free-reign on how to divide the work between
threads. Good load balancing, but incurs larger overhead.
- `guided`: starts with a large chunk size, then shrinks that eventually to some
user-defined minimum. May in certain cases offer better performance to dynamic
and ensures that even the minimum chunk size is still performant.

Lets consider an example where we want to square a triangular array of values
and see the performance of each of the three types of scheduling.

```cpp

```

### A few simple examples

#### Example 1

```cpp
#pragma omp parallel       // Begin parallel region, fork threads.
{
#pragma omp for            // Work sharing construct.
  for (i=0; i<n; ++i){
    a[i] = b[i]+c[i];
  }                        // Barrier here at end of work sharing construct
}                          // End parallel region, barrier here, merge threads.
```

#### Example 2

```cpp
#pragma omp parallel       // Begin parallel region, fork threads.
{
#pragma omp for            // Work sharing construct.
  for (i=0; i<n; ++i){
    a[i] = b[i]+c[i];
  }                        // Barrier here. Needed b/c next loop is dependent.

#pragma omp for            // Work sharing construct.
  for (i=0; i<n/2; ++i){
    a[i] = b[i]*d[i];
  }                        // Barrier here.
}                          // End parallel region, barrier here, merge threads.
```

#### Example 3

```cpp
#pragma omp parallel for     // Begin parallel region, and worksharing
{
  for (i=0; i<N; ++i){
    prod = prod + b[i]*c[i]  // Race condition!
  }
}
```
