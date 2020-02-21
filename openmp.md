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

```bash
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

There are three parts of `omp` pragma/compiler directive:

- The sentinal: `#pragma omp` which notifies the compiler "Hey `omp` stuff
here!"
- The construct: (e.g. `parallel`) which determines the envoked directive.
- The clause: `num_threads` which contains more specific information to comtrol
how the `construct` behaves.

The three of these together will generally appear together on a single line
`#pragma omp parallel 4` which defines the creation of a parallel region with
four threads.

But more details on this later. The important point is to understand the
components of an `omp` pragma/directive as more details on the syntax will
be provided later.

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
