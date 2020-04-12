# Synchronization

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

Since paralyzing serial code using `omp` requires the use of multiple threads
and these threads can be anywhere inside the `parallel region` or
`worksharing construct` we need additional constructs to help synchronize these
threads. The three synchronization constructs discussed below are `critical`,
`atomic`, and `master`.

## Critical

`Critical` is a synchronization construct which specifies that only a single
thread can execute the code in that scope at a time. Note this is different from
the `worksharing construct` `single` which specifies that only a single thread
executes the code and the others must wait at the barrier.

We have used `critical` extensively in the previous examples to "protect" print
statements. Note that if you create a `parallel region` and then immediately
define a `critical` region, then close the two regions, you have essentially
written very complicated serial code. This abuse of `critical` can be seen in
the very first `helloworld.cpp` example, though this was necessary to avoid the
race condition on the iostream.

## Atomic

This synchronization construct performs the same function of `critical`, however
`atomic` utilizes a special feature of your operating system and can outperform
`critical` when there is only a single simple statement.

Do not use `atomic` for multiple lines or complex operations (such as a
function call), as this will most certainly underperform a `critical`.

## Master

`Master` specifies work for the master thread (i.e., thread 0). It is similar
to `single` in that only a single thread executes the code, but `master` is not
a worksharing construct and therefore there is no implied barrier.

`master` is most commonly used to print logging information so that regardless
of whether the code is run in serial or parallel only a single set of logging
info is returned.
