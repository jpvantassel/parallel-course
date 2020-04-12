# Sharing Memory

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

## Private vs Shared Memory

One of the unique characteristics of `OpenMP` is the ability for different
threads to share access to the same memory. However, this opportunity also comes
with the risk that two (or more) threads may try to write to the same location
in memory in the incorrect order, this is called a `race condition`.

A `race condition` occurs when two (or more) threads attempt to utilize the
same resource. The resource is canonically thought of as a location in memory,
but it does not have to be, it could for example be an iostream. We saw this
already in the very first `helloworld.cpp` example, although we did not make
note of it.

Race conditions are problematic for several reasons which include:

1. Variables can become out of sync among the threads leading to inconsistent
state and incorrect results.
<!-- 2. TODO (jpv): Are there other reasons to avoid a race condition. -->

Below is an example of code __with__ a race conditions.

> racecondition.cpp

```cpp
#include <iostream>
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
penalty of a compile-time error) to decide all whether a variable is `shared`
or `private`.

Importantly, there are two exceptions to the `default` of `shared`. The first
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
  slow_func();                              // Separate threads using slow_func
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

> ./a.out

```bash
17,7,17,7,17,17,17,7,17,7,
18,8,18,8,18,18,18,8,18,8,
```

Where with the race conditions resolved we see that the __correct__ answer is.

> ./a.out

```bash
1,3,5,7,9,11,13,15,17,19,
2,4,6,8,10,12,14,16,18,20,
```
