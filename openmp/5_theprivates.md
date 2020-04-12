# The Privates

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

In the previous section, `private` was presented as a way to resolve race
conditions (i.e., inconsistent state amongst threads). However, a side effect
of using a `private` variable is that the value of the variable is not
instantiated for each thread by default. And furthermore the value at the end
of `parallel for` is not returned to the master thread.

To overcome there two specific problems, `omp` provides `firstprivate()` and
`lastprivate()`. Variables who are `firstprivate()` are initialized with the
value defined outside the `parallel region`. Variables who are `lastprivate()`
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
  psum += 1;                                  // Count to n using separate threads
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

<!-- TODO (jpv): Fix this example -->

Now lets examine an example for the use of `lastprivate()`.

> lastprivate.cpp

```cpp
#include <iostream>
#include "omp.h"

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
