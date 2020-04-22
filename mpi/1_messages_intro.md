# An Introduction to Message Passing

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

## Basic Concepts

- `MPI` uses message passing to share information between processes. The logic
behind which data is sent and received between processes is the primary concern
of the programmer.
- Message passing requires several basic components:
  - A method of communication
  - Description of what data is to be sent/received, includes type and size.
  - Where the data came from and where it is going.
  - Whether the message was received successfully.

## Channels of Communication

`MPI` allows the programmer to group messages into seperate channels, these
channels are called communicators. Think of a communicator as a channel of a
radio. Both the sender and the receiver need to be tuned to that channel in
order to pass information. You have already seen the default
communicator, `MPI_COMM_WORLD`, in our `hello.cpp` example. `MPI_COMM_WORLD` by
default includes all ranks (i.e., processes), however the programmer is free
to make subsets of `MPI_COMM_WORLD`.

<!-- See Day1 slide 38. If you create a subset of `MPI_COMM_WORLD`, do the processes
receive a new rank from the communicator or do they retain ther rank from
`MPI_COMM_WORLD`? -->

There are two very useful functions related to communicators:

- `int ierr = MPI_Comm_size(<comm MyComm>, <int *size>)`: Accepts the
communicator of interest and a pointer to an integer, sets the value of the
integer to the number of ranks in the communcator, and returns an error code.
- `int ierr = MPI_Comm_rank(<comm MyComm>, <int *rank>)`: Similiar to the
previous except it sets the value of the integer to the processes rank.

## Messaging Modes

Data must exist either on the sending processes memory or the receiving
processes memory. Stated another way there is no central hub to store data
between processes. This is problematic because it requires both the sender and
receiver to be prepared to communicate at the same time and if they are not
both ready the ready process must wait for the other. This type of communication
is refered to as `synchronous`, as both processes must be syncrhonized before
they can proceed. This is in contrast to `asynchronous`, which does not require
the two processes to become synced.

`MPI` leave the choice of how messages are sent to the programmer:

- Synchronous Send - Complete only after the data has been received.
- Buffered Send - Complete when data has been transferred to a buffer.
- Standard Send - Either synchronous or buffered.
- Ready Send - Complete after receive has been confirmed.

<!-- See Day3 slide 8. What is a ready send? -->

Note there is only a single function for receiving messages, and it accepts all
of the four send types.

## Basic Syntax

## Message Addressing

Message passing includes two general categories of communication
`point-to-point` and `group/collective`.

- `Point-to-point`: A message is passed from one process to another process.
- `Group/Collective Messages`: A message is sent
  - from many processes to a single process (i.e., many-to-one),
  - from a many processes to many other processes (i.e., many-to-many),
  - from a single process to many processes (i.e., one-to-many).


