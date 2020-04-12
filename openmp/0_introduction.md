# Introduction

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

## Basic Concepts

- __Parallel Region__: Area in which multiple threads may exist.
  - At the beginning threads are `forked`, and
  - At the end `threads` are merged.
- __Work Sharing__: The idea that some tasks can be broken up and performed at
the same time. Without work sharing `forked` threads perform `replicated work`.
- __Public/Private Data__: Threads may share some data in common (i.e.,
`Public Data`) or may keep their own private copy (i.e., `Private Data`).

## Compiling an OpenMP Program

- Include header with `#include "omp.h"`
- Compile with openmp flag
  - _gcc_ - `gcc foo.c -fopenmp`
  - _g++_ - `g++ foo.cpp -fopenmp`
  - _intel_ - `icc foo.c -openmp`

_When using the openmp compiler flag, the macro `_OPENMP` is defined,
with the OpenMP version number (e.g., '201511'). This may be useful to use as a
check whether or not the program being run has been compiled properly using
OpenMP._

## Running an OpenMP Program

- Launch a program in the standard way `./a.out`.
- However, keep in mind the program is sensitive to environment variables
including:
  - `OMP_NUM_THREADS`: Sets number of threads that may used.
  - `OMP_SCHEDULE`: Allows for experimenting with scheduling (more on this
  later).

## A Simple OpenMP Program

Below is a simple "Hello World" type program for `openmp`.

>helloworld.cpp

```cpp
#include <iostream>             // Include iostream for printing.
#include "omp.h"                // Include openmp header.

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
- To run `./a.out`.

If you compiled and ran it correctly you should see something like this ...

> ./a.out

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

_The number of threads you see will depend on the number of hardware
threads your machine supports by default. The machine I used for this test
supports by default 8, therefore I see 8 "Hello ...". Remember you can change
that default by setting the environment variable `OMP_NUM_THREADS` to any
positive integer (e.g., `export OMP_NUM_THREADS=4`)_

## Anatomy of an `omp` pragma

Now that we have seen an example let's dissect the basic syntax.

There are three parts of an `omp` pragma (also known as a compiler directive):

- The sentinel: `#pragma omp` which notifies the compiler "Hey `omp` stuff
here!"
- The construct: (e.g., `parallel`) which determines the evoked directive.
- The clause: (e.g., `for`) which contains more specific information to control
how the `construct` behaves.

The three of these together will generally appear together on a single line
`#pragma omp parallel for` which defines the creation of a parallel region
applied to a `for` loop.

But more details on this later. The important idea here is to understand the
components of an `omp` pragma rather than the syntax as other examples more
specific examples will be provided later.
