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
  - `OMP_SCHEDULE`: Allows for experimenting with scheduling (more on this
  later).

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

## Api Basics

The `omp` API (Application Programming Interface) includes a number of helpful
built in functions. You have seen a few of these previously for example
`omp_get_thread_num()` in the above `helloworld.cpp` example. A thourough
introduction to the basic api calls are presented here. It is important to spend
a some time to understand how these calls behave when inside and outside the
`parallel region` (as we will show these may be different) in order to
understand the remaining examples in this module.

The below example includes the basic api calls with their reults shown below.

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

>./a.out

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

>numthreads.cpp

```cpp
#include <iostream>
#include "omp.h"

int main(){

std::cout<<"Serial Region:"<<std::endl;
std::cout<<"  omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"  omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

omp_set_num_threads(8);
std::cout<<"omp_set_num_threads(8)"<<std::endl;
std::cout<<std::endl;

std::cout<<"  Serial Region:"<<std::endl;
std::cout<<"    omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"    omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

#pragma omp parallel
{
#pragma omp single
{
std::cout<<"  Parallel Region:"<<std::endl;
std::cout<<"    omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"    omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
std::cout<<std::endl;

omp_set_num_threads(6);
std::cout<<"omp_set_num_threads(6)"<<std::endl;
std::cout<<std::endl;

std::cout<<"  Parallel Region:"<<std::endl;
std::cout<<"    omp_get_max_threads()  "<<omp_get_max_threads()<<std::endl;
std::cout<<"    omp_get_num_threads()  "<<omp_get_num_threads()<<std::endl;
}
}

}
```

>>./a.out

```bash
Serial Region:
  omp_get_max_threads()  4
  omp_get_num_threads()  1

omp_set_num_threads(8)

  Serial Region:
    omp_get_max_threads()  8
    omp_get_num_threads()  1

  Parallel Region:
    omp_get_max_threads()  8
    omp_get_num_threads()  8

omp_set_num_threads(6)

  Parallel Region:
    omp_get_max_threads()  6
    omp_get_num_threads()  8
```

In summary

- `omp_in_parallel()`: 0 if in serial region, 1 otherwise (parallel region and
worksharing).
- `omp_get_num_procs()`: Always the number of processors (i.e., maximum number
of hardware threads).
- `omp_get_num_threads()`: Number of currenlty spawned threads, 1 if in serial
region.
- `omp_get_thread_num()`: 0 if in serial region, 0 to N-1 otherwise. Where N is
the number of avaiable threads (i.e., equal to `omp_get_num_threads`).
- `omp_get_max_threads()`: Depends on the following listed in order of highest
to lowest precidence `omp_set_num_threads(#)`>`OMP_NUM_THREADS`>
`omp_get_num_procs()`.
  - The default is the number of hardware threads available (i.e., equal to
  `omp_get_num_procs()`).
  - If the environment variable `OMP_NUM_THREADS` has been set then it is replace
  with that value.
  - If the `omp_set_num_threads(#)` api call has been made then it returns `#`,
  ignoring the value from `OMP_NUM_THREADS` if it has been set.

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

The schedule types listed above each come with a `chunk size` arguement, they
are detailed as follows.

- `schedule(static, N)`: Each thread receives N iterations starting from thread
0 and increasing to N-1 threads. If there is more work it begins again from 0.
If N is not specified then the work is divided as equally as possible in terms
of loop iterations (e.g., if there are 100 iterations and 4 threads each thread
would recieve 25).
- `schedule(dynamic, N)`: Each thread receives N iterations however the thread
order is decided at runtime. If N is not specifies a default `chunk size` is
assumed this may be 1.
- `scheudle(guided, N)`: `omp` decides how much work should go to each thread.
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
#include <omp.h>

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
  Thread Number: 0 i=8
  Thread Number: 0 i=9
  Thread Number: 3 i=6
  Thread Number: 3 i=7
  Thread Number: 1 i=2
  Thread Number: 1 i=3
  Thread Number: 2 i=4
  Thread Number: 2 i=5
  Thread Number: 1 i=10
  Thread Number: 1 i=11
schedule(dynamic,2)
  Thread Number: 0 i=0
  Thread Number: 0 i=1
  Thread Number: 1 i=2
  Thread Number: 1 i=3
  Thread Number: 3 i=6
  Thread Number: 3 i=7
  Thread Number: 1 i=10
  Thread Number: 1 i=11
  Thread Number: 0 i=8
  Thread Number: 0 i=9
  Thread Number: 2 i=4
  Thread Number: 2 i=5
schedule(guided, 2)
  Thread Number: 1 i=0
  Thread Number: 1 i=1
  Thread Number: 1 i=2
  Thread Number: 3 i=8
  Thread Number: 3 i=9
  Thread Number: 1 i=10
  Thread Number: 1 i=11
  Thread Number: 2 i=3
  Thread Number: 2 i=4
  Thread Number: 2 i=5
  Thread Number: 0 i=6
  Thread Number: 0 i=7
```

Now lets compare performance between the three schedule types, when we want to
initialize a triangular matrix (i.e., an unbalanced load condition).

> compareschedule.cpp

```cpp
#include "omp.h"
#include "iostream"
#include "chrono"

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

## Data Scoping

### Shared and Private

To prevent race conditions `omp` allows us to specify `private` memory to each
thread.

By default `omp` assumes all variables are `shared`, because shared variables
cut down on wasted memory. Note that you can change the default from `shared` to
`private` by using the `default` clause, however this is fairly uncommon and
recommended. What is more common and shown in many of the examples below is to
use the `default(none)` syntax. Using this clause forces the programmer (on
penealty of a compile-time error) to decide all whether a variable is `shared`
or `private`.

Importantly, there are two exeptions to the `default` of `shared`. The first
is the first loop index of a `parallel for` `worksharing` region. There are few
(if any) compelling reasons that this variable should ever be public, so its
a convenience for `omp` to make this variable private. The second, is any
stack variable declared inside of a parallel region.

Below is an example illustrating the use of `shared` and `private`.

> private.cpp

```cpp
#include "omp.h"
#include "iostream"
#include "thread"
#include "chrono"

void slow_func(){                           // Define a slow function.
  std::chrono::milliseconds timespan(5);
  std::this_thread::sleep_for(timespan);
}

int main(){

const int n = 10;                           // Declare arrays
int *evens = new int [n];
int *odds = new int [n];

int even;                                   // Declare even and odd
int odd;

#pragma omp parallel default(none) private(even, odd) shared(evens, odds)
{
#pragma omp for
for (int i=0; i<n; i++){  
  even = 2*i+2;                             // ith even number to even
  slow_func();                              // Seperate threads using slow_func
  odd =  even-1;                            // ith odd number to odd
  evens[i] = even;                          // Assign even and odd
  odds[i] = odd;
}
}

for (int i=0; i<n; i++){                    // Print evens
  std::cout<<evens[i]<<",";
}
std::cout<<std::endl;
for (int i=0; i<n; i++){                    // Print odds
  std::cout<<odds[i]<<",";
}
std::cout<<std::endl;

delete[]evens;                              // Clean up the arrays
delete[]odds;
}
```

If we did __not__ make `even` and `odd` private (and we using multiple threads),
we would get undefined results. A possible __wrong__ result for example could
be.

>./a.out

```bash
17,7,17,7,17,17,17,7,17,7,
18,8,18,8,18,18,18,8,18,8,
```

Where with the race conditions resolved we see that the __correct__ answer is.

>./a.out

```bash
1,3,5,7,9,11,13,15,17,19,
2,4,6,8,10,12,14,16,18,20,
```

### First Private and Last Private

In the previous section, `private` was presented as a way to resolve race
conditions (i.e., inconsistnet state amoungst threads). However, a side effect
of using a `private` variable is that the value of the variable is not
instantiated for each thread by default. And furthermore the value at the end
of `parallel for` is not returned to the master thread.

To overcome there two spefic problems, `omp` provides `firstprivate()` and
`lastprivate()`. Variables who are `firstprivate()` are initialized with the
value defined outside the `parallel region`. Varaibles who are `lastprivate()`
have the value at the end of the last iteration of the `parallel for` returned
to the master thread for use in subsequent serial or parallel regions.

Below is an example where the code, as written, does not have a `race condition`
but it also does not return the correct results (i.e., the result we would get
if we ran it serially).

> firstprivate.cpp

```cpp
#include "iostream"
#include "omp.h"

int main(){

const int n = 80;                             // Make psum=10 (b/c 8 threads)
int psum=5;                                   // Add initial value so psum=15

#pragma omp parallel private(psum)            // Making psum private is a MISTAKE!
{

#pragma omp for
for (int i=0; i<n; i++){
  psum += 1;                                  // Count to n using seperate threads
}

#pragma omp for
for (int i=0; i<omp_get_num_threads(); i++){

#pragma omp critical                          // Use critical to avoid race condition
std::cout<<psum<<", ";                        // Print partial sums from each thread

}

}

}
```

The result of running this program is shown below. We see that the sum of all
of the partial sums (`psum`) are 80, however that is not what we expected, as
`psum` was initialized to `5`. The issue here is since `psum` is private
it does not get initialized in the `parallel region`.

> ./a.out

```bash
10, 10, 10, 10, 10, 10, 10, 10,
```

To get the expected result we need to replace `private(psum)` in the above
code to `firstprivate(psum)` then we will get the expected result.

> ./a.out

```bash
15, 15, 15, 15, 15, 15, 15, 15,
```

Now lets examine an example for the use of `lastprivate()`.

> lastprivate.cpp

```cpp
#include "omp.h"
#include "iostream"

int main(){

const int n=10;                             // Define iterations
int var=24;                                 // Define variable

std::cout<<"Serial Region - Variable var="<<var<<std::endl;

#pragma omp parallel default(none) shared(var, std::cout)
{

#pragma omp single
std::cout<<"Parallel Region - Variable var="<<var<<std::endl;

#pragma omp for schedule(dynamic, 3) private(var)
for (int i=0; i<n; i++){
  var = i;
  #pragma omp critical
  {
  std::cout<<"Thread Number: "<<omp_get_thread_num()<<"\t"
           <<"Variable var="<<var<<std::endl;
  }
}                                           // End parallel for

#pragma omp single
std::cout<<"Parallel Region - Variable var="<<var<<std::endl;

}                                           // End parallel region

std::cout<<"Serial Region - Variable var = "<<var<<std::endl;

}
```

When `export OMP_NUM_THREADS=4` this returned.

> ./a.out

```bash
Serial Region - Variable var=24
Parallel Region - Variable var=24
Thread Number: 1 - Variable var=0
Thread Number: 1 - Variable var=1
Thread Number: 1 - Variable var=2
Thread Number: 2 - Variable var=3
Thread Number: 2 - Variable var=4
Parallel Region - Variable var=24
Serial Region - Variable var = 24
```

We observe that by making `var` private within the for loop preventing the
result from being returned into the parallel region's scope.

Now if we change `private(var)` to `lastprivate(var)`, we see the following.

> ./a.out

```bash
Serial Region - Variable var=24
Parallel Region - Variable var=24
Thread Number: 2 - Variable var=0
Thread Number: 2 - Variable var=1
Thread Number: 2 - Variable var=2
Thread Number: 1 - Variable var=3
Thread Number: 1 - Variable var=4
Parallel Region - Variable var=4
Serial Region - Variable var = 4
```

In this case we do see that `var` is returned out of the `parallel for` scope
and back into the `parallel region`.

### Reduction

To make the programmers life easier, `omp` allows for a common operation
refered to as a reduction to be done almost automatically without any
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
the final value of `sum` is equal to 36 (the intial value 1 + the sum of `array`
which was equal to 35).

## Synchronization

Since parallizing serial code using `omp` requires the use of multiple threads
and these threads can be anywhere inside the `parallel region` or
`worksharing construct` we need additional constructs to help synchonize these
threads. The three synchonization constructs discussed below are `critical`,
`atomic`, and `master`.

### Critical

`Critical` is a synchronization construct which specifies that only a single
thread can execute the code in that scope at a time. Note this is different from
the `worksharing construct` `single` which specifies that only a single thread
executes the code and the others must wait at the barrier.

We have used `critical` extensively in the previous examples to "protect" print
statements. Note that if you create a `parallel region` and then immediately
define a `critical` region, then close the two regions, you have essentially
written very complicated serial code. This abuse of "critical" can be seen in
the very first example `helloworld.cpp`.

### Atomic

This synchonization construct performs the same function of `critical`, however
`atomic` utilizes a special feature of your operating system and can outperform
`critical` when there is only a single simple statement.

Do not use `atomic` for multiple lines or complex operations (such as a
function call), as this will most certainly underperform a `critical`. So use
`critical` instead.

### Master

`Master` specifies work for the master thread (i.e., thread 0). It is similar
to `single` in that only a single thread executes the code, but `master` is not
a worksharing construct and therefore there is no implied barrier.

`master` is most commonly used to print logging information so that regardless
of whether the code is run in serial or parallel only a single set of logging
info is returned.

## Orphaned Worksharing

`omp` allows the definition of `parallel region` to be seperated from the
corresponding `worksharing construct`. This allows the programmer to build
general functions/classes that can be executed in serial or parallel.

To illustrate a simple table is shown below to illustrate the interactions
between the `parallel region` and `worksharing construct`.

|                   | Parallel - Yes    | Parallel - No         |
|:-----------------:|:-----------------:|:---------------------:|
| Worksharing - Yes | Parallel          | Orphaned Worksharing  |
| Worksharing - No  | Replicated Work   | Serial                |

## Tasking

`Tasking` is an alternative to `worksharing`. It is useful for many cases
which `worksharing` cannot be used for example while loops, navigating trees, and work with dependencies.

### A Task

Tasks:

- Are generated with the `#pragma omp task` construct, they
- Include a statement or collection of statements to be executed.
- __Can__ be performed immediately __or__ deferred (placed on
a queue). Regardless of whether a task is performed immediately or
deferred must not effect the result.
- Can be split into two types:
  - Implicit tasks: These include the work of threads in a parallel 
  region. This is not what we are discussing here.
  - Explicit tasks: These are the type denoted with the `task` construct.
  This is what we are discussing here.

### Task Synchronization

Sometimes it is necessary to ensure all of the task are at the same point
in the program before another statement can be executed. This is refered
to as `task synchronization` and can be done using several `contructs`.

- `#pragma omp taskwait`: Wait for completion of all sibling.
- `#pragma taskgroup {}`: Wait for all sibling and their descendents.

### Creating Tasks

Creating tasks in the serial region is simple (and pointless), and is
shown below in a simple example.

TODO (jpv): Add simpler serial example of tasking.

> serialtask.cpp

```cpp
#include <iostream>
#include <omp.h>

int main(){
  
#pragma omp task
{
  std::cout<<"This is a task!";
}

}
```

Creating them in a parallel region is slightly more complex. First, spawn
threads by creating a `parallel region`. Second, define a task generating
thread by defining a `single` region (only one thread is in charge of
creating tasks for the other N-1 threads). Third, define tasks within that
`single` region. As tasks are generated by the generating thread (not
necessarily the master thread), tasks are either executed immediately or
are deferred (i.e., left to the other N-1 threads to pickup as they
become available).

TODO (jpv): Add example of parallel tasking.

### Task Data Sharing

- Shared variables in `parallel region` remain shared.
- Loop index varaibles remain `private`.
- `Private` variables become `first private` for the task to ensure
they are properly initialized. Important for deferred tasks which may need
information from when the task was generated (and placed on the queue) and
not the value (that may have changed since then) when it is run (taken)
off of the queue.

### Scheduling Tasks

We have discussed how tasks are created and that these tasks may either
be performed immediately or queued, however we have not discussed in what
order these tasks are performed or how it is decided when a task should be
performed immediately or deferred. To disucss these details we must
first understand the ways `omp` schedules tasks and how the user can
encourage certain behviors.

One of the easiest ways to ensure a task is performed before others
waiting in the queue is with `priority`. When we create higher priority
tasks by increasing their number for example
`#pragma omp task priority(5)` has a higher priority than
`#pragma omp task priority(4)`. Listing the priority will encourage
`omp` to run the first `priority(5)` task before the `priority(4)` task
although this is not guaranteed. To guarantee one task is performed before
another see depdencey section below.

To control whether a task is performed immediately or defered you can use
the `if() clause`. When the statement within the `if() clause` is false,
the task is run immediately by the generating thread
(i.e., a task is not created).

### Task Dependency

Some tasks will be dependent on other tasks, to pass this information
along to `omp` we use the `depend()` clause.

The depend clause is as follows `depend( dependence-type : list )` where
the `depenence-type` can be on the following:

- `in` : Think of as a read dependency.
- `out`: Think of as a write dependecy.
- `inout`: Think of a read then write dependency.

And where `list` is an item requiring one of the aforementioned
dependencys.

There are four combinations of two dependencies:

- __Read after Write (RaW)__ Write variable then read.
- __Write after Read (WaR)__ Read variable then write.
- __Write after Write (WaW)__ Write then Write.
- __Read after Read__ No dependence between tasks b/c read only.
