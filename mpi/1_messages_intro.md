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

`MPI` allows the programmer to group messages into separate channels, these
channels are called communicators. Think of a communicator as a channel on a
radio. Both the sender and the receiver need to be tuned to that same channel in
order to pass information. You have already seen the default
communicator, `MPI_COMM_WORLD`, in our `hello.cpp` example. `MPI_COMM_WORLD` by
includes all ranks (i.e., processes), however the programmer is free
to make subsets of `MPI_COMM_WORLD`.

<!-- See Day1 slide 38. If you create a subset of `MPI_COMM_WORLD`, do the processes
receive a new rank from the communicator or do they retain their rank from
`MPI_COMM_WORLD`? -->

There are two very useful functions related to communicators:

- `int ierr = MPI_Comm_size(<comm MyComm>, <int *size>)`: Accepts the
communicator of interest and a pointer to an integer. Sets the value of the
integer to the number of ranks in the communicator, and returns an error code.
- `int ierr = MPI_Comm_rank(<comm MyComm>, <int *rank>)`: Similar to the
previous except it sets the value of the integer to the processor's rank.

## Messaging Modes

Data must exist either on the sending processes memory or the receiving
processes memory. Stated another way, there is no central hub to store data
between processes. This is problematic because it requires both the sender and
receiver to be prepared to communicate at the same time and if they are not,
the process ready first must wait for the other. This type of communication
is referred to as `synchronous`, as both processes must be synchronized before
they can proceed. However, `MPI` provides us an alternative called
`asynchronous` communication, which does not require the two processes to be
ready at the same time. We introduce both methods of communication below.

### Synchronous Communication

Here is a list of `synchronous` sends:

- Synchronous Send (MPI_Ssend) - Complete only after the last packet of data
has been sent.
- Buffered Send (MPI_Bsend) - Complete when data has been transferred to a
buffer.
- Standard Send (MPI_Send) - Either synchronous or buffered.
- Ready Send (MPI_Rsend) - Complete after receive has been confirmed.

<!-- See Day3 slide 8. What is a ready send? -->

_Note there is only a single function for receiving messages, and it accepts all
of the four send types._

There are several problems with synchronous communication, they include:

- Both sender and receiver must at some point in time become synchronized or
rely on a buffer.
- Synchronization can be __slow__, because one process needs to wait for
another, and __may result in deadlocks__, because process one is waiting to send
data to process two while process two is waiting to send data to process one.
- Buffers only work for smaller message sizes because large message sizes
require larger buffers and large buffers require a large memory footprint.
- Buffers must be checked periodically to ensure they do not overflow.

### Asynchronous Communication

Asynchronous communication is syntactically must simpler than synchronous
communication. It includes only a single send command (MPI_Isend) and a single
receive command (MPI_Irecv). The only additional wrinkle of complexity is
MPI_Wait, discussed below.

Basic structure of an asynchronous send, is shown below in pseudo-code.

```cpp
// Define an array called `array`
MPI_Isend()             // Immediate send of some elements of `array`
// Do other work that does not use `array`
MPI_Wait()              // Process will wait here until communication is done
// Work with `array`
```

The above example shows the case where we define an array of elements called
`array`. We wish to send some of those elements to another process. For example,
this could used for sharing the boundary elements of an array, in a domain
decomposition problem. We use an immediate send (Isend) to share the
information. This notifies the receiver that data is ready to be sent. If the
receiver is ready, the transfer begins, if not then the sender moves onto its
other work that does not include `array`. The sender continues performing this
other work until it reaches `MPI_Wait()`. When it reaches the wait statement the
sender check if the message has been transferred, if it has the sender is free
to move past the wait to the rest of the program, if not, then it must wait for
the receiver to be ready.

This process also applies to the receiver in a similar manner. The basic
structure of an asynchronous receiver, is shown below in pseudo-code.

```cpp
// Define an array called `array`
MPI_Irecv()             // Immediate receiver, see if data is ready to send
// Do other work that does not use `array`
MPI_Wait()              // Process will wait here until data is sent
// Work with `array`
```

## Basic Syntax

## Message Addressing

Message passing includes two general categories of communication
`point-to-point` and `group/collective`.

- `Point-to-point`: A message is passed from one process to another process.
- `Group/Collective Messages`: A message is sent
  - from many processes to a single process (i.e., many-to-one),
  - from a many processes to many other processes (i.e., many-to-many),
  - from a single process to many processes (i.e., one-to-many).
