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
  - _gcc_ - `gcc foo.c -fopenmp`
  - _g++_ - `g++ foo.cpp -fopenmp`
  - _intel_ - `icc foo.c -openmp`

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

>helloworld.cpp

```cpp
#include "omp.h"                // Include openmp header.
#include "iostream"             // Include iostream to print.

int main(){
#pragma omp parallel            // Begin parallel region, threads forked here.
{
#pragma omp critical            // Begin critical region, single thread at time.
{
std::cout<<"Hello from thread "
         <<omp_get_thread_num() // omp function to get current thread identifier.
         <<"!"
         <<std::endl;
}
}
}
```

- To compile `g++ helloworld.cpp -fopenmp`.
- To run `./a.out`.

If you compiled and ran it correclty you should see something like this ...

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

_Note: The number of threads you see will depend on the number of hardware
threads your machine supports by default. The machine I used for this test
supports by default 8, therefore I see 8 hellos. Remember you can change that
default by setting the enviornment variable `OMP_NUM_THREADS`
(e.g., `export OMP_NUM_THREADS=4`)_

## Anatomy of an `omp` pragma

Now that we have seen an example let's disect the basic syntax.

There are three parts of an `omp` pragma (also called a compiler directive):

- The sentinal: `#pragma omp` which notifies the compiler "Hey `omp` stuff
here!"
- The construct: (e.g. `parallel`) which determines the envoked directive.
- The clause: (e.g. `for`) which contains more specific information to comtrol
how the `construct` behaves.

The three of these together will generally appear together on a single line
`#pragma omp parallel for` which defines the creation of a parallel region
applied to a `for` loop.

But more details on this later. The important idea here is to understand the
components of an `omp` pragma rather than the syntax as other examples will
be provided later.

## The parallel region

- A parallel regions begins with `#pragma omp parallel`.
- At the start of the parallel region the threads are forked.
- At the end of the parallel region is an implied barrier.
- Once all threads reach this implied barrier the threads are merged.

A simple example would be:

> parallelregion.cpp

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

> ./a.out

```bash
Serial Code Before
Parallel Code
Parallel Code
Parallel Code
Parallel Code
Serial Code After
```

## Work Sharing

After the `fork` and `join` model the next most significant concept to
understand is `work sharing`. Without worksharing we cannot
utilize the threads we `fork` at the beginning of a parallel region, and can
only perform `replicated work`.

We saw an example of `replicated work` in the previous example where we printed
"Parallel Code" `N` times where `N` was the number of threads. We will
generally not want to perform `replicated work` (because for most problems it
will do as much work as serial code but take longer than serial code because of
the added overhead). Instead we want to use `work sharing` to increase our
code's performance.

All worksharing constructs in `omp` are:

1. intended to prevent `replicated work`, and
2. have an implied barrier at the end of their scope.

### Parallel For

One of the most common `work sharing` constructs is `parallel for` which
allows for-loops to run in parallel (more details on this to follow). For
now lets consider a simple `for` loop which can be parallelized.

> for.cpp

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

>./a.out

```bash
2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
```

_Note: When using `parallel for` you cannot use `break` statements
(compiler error), but you can use `continue` to achieve similar results._

### Single

The next most widely used `worksharing` construct is `single`. As the name
implies, code within a `single` worksharing region is only executed by one and
only one thread. Since `single` is a worksharing construct the other threads
must wait at the `single` regions barrier.

> single.cpp

```cpp
#include "iostream"
#include "omp.h"

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

TODO (jpv): Come up with a less trivial example.

When I executed this program for the first time, it returned the following.

>./a.out

```bash
7                                           # Thread number between 0 and N-1
```

Note that since my machine has 8 hardware threads (and I have not defined the
environment variable `OMP_NUM_THREADS`), re-running `a.out` can return any value
between 0 and N-1. But it will only ever return a single value per run because
of our use of the `single` construct.

## Work Schedule

In the previous examples we were performing parallel work, but we did not
concern ourselves with how the work was divided between threads. In this
section we will learn how to dictate to `omp` how to divide the work.

Work can be divided in one of three ways:

- `static`: All threads receive equal work. Fast and simple.
- `dynamic`: Allows the scheduler free-reign on how to divide the work between
threads. Good load balancing, but costs more overhead than `static`.
- `guided`: The scheduler begins with a large chunk size, and shrink it unitil
it reaches the user-defined minimum. `guided` may in certain cases offer better
performance than `dynamic`. Set the minimum chunk size such that the
parallelization is still performant.

Lets consider an example where we want to square a triangular array of values
and see the performance of each of the three types of scheduling.

> schedule.cpp

```cpp
#include "omp.h"
#include "iostream"
#include "chrono"

int main(){

const int nrows=10000;                  // Define triangular array 10000 x 10000.
int **rows = new int*[nrows];
int **tmps = new int*[nrows];

for (int i=0; i<nrows; ++i){            // Setup the triangular nature.
  rows[i] = new int [i];
  tmps[i] = new int [i];
}

// Static
auto start_static = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(static)
for (int i=0; i<nrows; ++i){
  for (int j=0; j<i; ++j){
    tmps[i][j] = rows[i][j]*rows[i][j];
  }
}
auto end_static = std::chrono::high_resolution_clock::now();
double elapsed_time_static = std::chrono::duration_cast<std::chrono::nanoseconds>(end_static-start_static).count();
elapsed_time_static *= 1e-9;
std::cout<<"Elapsed Time Static "<<elapsed_time_static<<" sec."<<std::endl;

// Dynamic
auto start_dynamic = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(dynamic)
for (int i=0; i<nrows; ++i){
  for (int j=0; j<i; ++j){
    tmps[i][j] = rows[i][j]*rows[i][j];
  }
}
auto end_dynamic = std::chrono::high_resolution_clock::now();
double elapsed_time_dynamic = std::chrono::duration_cast<std::chrono::nanoseconds>(end_dynamic-start_dynamic).count();
elapsed_time_dynamic *= 1e-9;
std::cout<<"Elapsed Time Dynamic "<<elapsed_time_dynamic<<" sec."<<std::endl;

// Guided
auto start_guided = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(guided, 1)
for (int i=0; i<nrows; ++i){
  for (int j=0; j<i; ++j){
    tmps[i][j] = rows[i][j]*rows[i][j];
  }
}
auto end_guided = std::chrono::high_resolution_clock::now();
double elapsed_time_guided = std::chrono::duration_cast<std::chrono::nanoseconds>(end_guided-start_guided).count();
elapsed_time_guided *= 1e-9;
std::cout<<"Elapsed Time Guided "<<elapsed_time_guided<<" sec."<<std::endl;

for (int i=0; i<nrows; ++i){            // Deallocate memory.
  delete[]tmps[i];
  delete[]rows[i];
}
delete[]tmps;
delete[]rows;
}
```

>./a.out

```bash
Elapsed Time Static 0.398055 sec.
Elapsed Time Dynamic 0.0430165 sec.
Elapsed Time Guided 0.0428927 sec.
```

## Private vs Shared Memory

One of the unique charachteristics of `OpenMP` is the ability for different
threads to share access to the same memory. However, this opportunity also comes
with the risk that two (or more) threads may try to write to the same location
in memory at the same time, this is called a `race condition`.

A `race condition` occurs when two (or more) threads attempt to utilize the
same resource. The resource is canonically thought of as a location in memory,
but it does not have to be, it could for example be an iostream. We saw this
already in the example `parallelregion.cpp`, although we did not make a note of
it.

Race conditions are problematic for several reasons which include:

1. Variables can become out of sync among the threads leading to inconsistent
state and incorrect results.
2. TODO (jpv): Are there other reasons to avoid a race condition.

Below is an example of code __with__ a race conditions.

> raceconditon.cpp

```cpp
#include "iostream"
#include "omp.h"

int main(){

const int n=10;
auto a = new int[n];        // Create an integer array  

a[0] = 2;                   // Initialize the first value
#pragma omp parallel for    // Begin parallel region and work sharing
for (int i=1; i<n; i++){    // Loop across array
  a[i] = a[i-1]*2;          // Race Condition!
}

for (int i=0; i<n; i++){    // Loop across array
  std::cout<<a[i]<<", ";    // Print each value
}
std::cout<<std::endl;

delete[] a;                 // Clean up dynamic array
}
```

This code has undefined behavior when run in parallel. When I run this code
with `OMP_NUM_THREADS=2` I got.

> ./race.out

```bash
2, 4, 8, 16, 32, 64, 0, 0, 0, 0,
```

But when I set `OMP_NUM_THREADS=1` (forcing the code in serial), I get
the correct result.

> ./race.out

```bash
2, 4, 8, 16, 32, 64, 128, 256, 512, 1024,
```

This inconsistency between the serial and parallel versions of the code is
undesirable. The __corrected__ code that does __not__ have a race condition is
shown below.

> noracecondition.cpp

```cpp
#include "iostream"
#include "omp.h"
#include "cmath"

int main(){

const int n=10;
auto a = new int[n];        // Create an integer array  

#pragma omp parallel for    // Begin parallel region and work sharing
for (int i=0; i<n; i++){    // Loop across array
  a[i] = std::pow(2, i)*2;  // Remove Race Condition!
}

for (int i=0; i<n; i++){    // Loop across array
  std::cout<<a[i]<<", ";    // Print each value
}
std::cout<<std::endl;

delete[] a;                 // Clean up dynamic array
}
```

You can confirm that when you use this fixed code that you do in fact get the
correct result no matter the number of threads used.
