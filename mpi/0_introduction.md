# Introduction

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

## Basic Concepts

- `MPI` which stands for `Message Passing Interface` is a
`distributed memory` programming model.
- Improved performance is attained by dividing a task up into
multiple nearly independent processes that can be performed in parallel
(i.e., `process-based parallelism`).
- The memory of each process is private (i.e., cannot be viewed by the
other processes), however `MPI`, as its name implies, facilitates messages
(i.e., data) to be passed between processes to allow multiple processes
distributed across multiple computers (i.e., nodes) to work together.
- The key to fast `MPI` programs is to maximize the amount of work that
can be performed in parallel and to limit the number and size of
messages to be passed.

## Writing A Simple MPI Program

Let's start with a simple example. Below is a "HelloWorld"-style program that
will introduce the basic structure of an `MPI` program.

> hello.cpp

```cpp
#include <iostream>                             // Include for printing
#include "mpi.h"                                // Include to use MPI functions

int main(){
int ierr, nranks, rank;                         // Allocate variables
ierr = MPI_Init(NULL, NULL);                    // Start MPI here

ierr = MPI_Comm_size(MPI_COMM_WORLD, &nranks);  // Number of processes
ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);    // Rank of current process
std::cout<<"Hello from process rank "
         <<rank<<" of "<<nranks<<std::endl;     // Have each process say hello

ierr = MPI_Finalize();                          // End MPI here
}
```

The above program has three stages:

- __SetUp__: Begins at the start of `main()`, and includes any initial
work to be done by all processes. We use this region to allocated three
integers, it concludes with the call to `MPI_Init()`.
- __Application__: Contains the time-consuming functions of the program that
are to be performed in parallel, it ends just before `MPI_Finalize`.
- __TearDown__: Begins with `MPI_Finalize` and concluded as the end of main. It
contains any final actions that need to be performed. This region will typically
be empty.

## Compiling the Program

To run our `hello.cpp` example, we need to first compile it. This can be
done with the following:

```bash
mpic++ hello.cpp
```

This will result in an executable `a.out`.

## Running the Program

If we run our program like a normal executable it will run as serial code
(i.e., only launch a single process). See below.

> ./a.out

```bash
Hello from process rank 0 of 1
```

To tell `MPI` how many processes we would like to use, we need to use
the launcher program `mpirun` (or a variant thereof such as `ibrun` which is
a wrapper of `mpirun` for intel machines).

> mpirun -np 4 ./a.out

```bash
Hello from process rank 1 of 4
Hello from process rank 0 of 4
Hello from process rank 2 of 4
Hello from process rank 3 of 4
```

The result shows that our program launched successfully with the number of
processors (`-np`) equal to 4. And so we have successfully written, compiled,
and run our first program using `MPI`.
