# OpenMP

> Joseph P. Vantassel, The University of Texas at Austin

## Basic Concepts

- __Parallel Region__: Area in which multiple threads may exist. At the
beginning threads are `forked` and at the end `threads` are merged.
- __Work Sharing__: The idea that some tasks can be broken up and performed at
the same time. If this were not possible the `forked` threads would perform
`replicated work`.
- __Public/Private Data__: Threads may share some data in common (i.e.,
`Public Data`) or may keep their own private copy (i.e., `Private Data`).

## Compiling an OpenMP Program

- Include header with `#include "omp.h"`
- Compile with openmp flag
  - _gcc_:`gcc foo.c -fopenmp`
  - _intel_: `icc foo.c -openmp`

_Note: When using the openmp compiler flag, the variable `_OPENMP` is defined,
and available for compiler directives if necessary._

## Running an OpenMP Program

- Launch a program in the standard way `./a.out`.
- However, the program is senstivate to environment variables including:
  - `OMP_NUM_THREADS`: Sets number of threads that my used.

TODO (jpv): Add other relavant environment variables.

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
