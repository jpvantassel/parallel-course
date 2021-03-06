# Tasking

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

`Tasking` is an alternative to `worksharing`. It is useful for many cases
which `worksharing` cannot be used for example while loops, navigating trees,
and work with dependencies.

## A Task

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

## Task Synchronization

Sometimes it is necessary to ensure all of the task are at the same point
in the program before another statement can be executed. This is referred
to as `task synchronization` and can be done using several `constructs`.

- `#pragma omp taskwait`: Wait for completion of all sibling.
- `#pragma taskgroup {}`: Wait for all sibling and their descendants.

## Creating Tasks

Creating tasks in the serial region is simple (and pointless), and is
shown below in a simple example.

<!-- TODO (jpv): Add simpler serial example of tasking. -->

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

<!-- TODO (jpv): Add example of parallel tasking. -->

## Task Data Sharing

- Shared variables in `parallel region` remain shared.
- Loop index variables remain `private`.
- `Private` variables become `first private` for the task to ensure
they are properly initialized. Important for deferred tasks which may need
information from when the task was generated (and placed on the queue) and
not the value (that may have changed since then) when it is run (taken)
off of the queue.

## Scheduling Tasks

We have discussed how tasks are created and that these tasks may either
be performed immediately or queued, however we have not discussed in what
order these tasks are performed or how it is decided when a task should be
performed immediately or deferred. To discuss these details we must
first understand the ways `omp` schedules tasks and how the user can
encourage certain behaviors.

One of the easiest ways to ensure a task is performed before others
waiting in the queue is with `priority`. When we create higher priority
tasks by increasing their number for example
`#pragma omp task priority(5)` has a higher priority than
`#pragma omp task priority(4)`. Listing the priority will encourage
`omp` to run the first `priority(5)` task before the `priority(4)` task
although this is not guaranteed. To guarantee one task is performed before
another see dependency section below.

To control whether a task is performed immediately or deferred you can use
the `if() clause`. When the statement within the `if() clause` is false,
the task is run immediately by the generating thread
(i.e., a task is not created).

## Task Dependency

Some tasks will be dependent on other tasks, to pass this information
along to `omp` we use the `depend()` clause.

The depend clause is as follows `depend( dependence-type : list )` where
the `dependence-type` can be on the following:

- `in` : Think of as a read dependency.
- `out`: Think of as a write dependency.
- `inout`: Think of a read then write dependency.

And where `list` is an item requiring one of the aforementioned
dependencies.

There are four combinations of two dependencies:

- __Read after Write (RaW)__ Write variable then read.
- __Write after Read (WaR)__ Read variable then write.
- __Write after Write (WaW)__ Write then Write.
- __Read after Read__ No dependence between tasks b/c read only.
