# Asynchronous Communication

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

## Basic Syntax

There are three main function calls to perform asynchronous communication:

- `MPI_Isend(buffer, count, datatype, dest, tag, comm, request)`
- `MPI_Irecv(buffer, count, datatype, source, tag, comm, request)`
- `MPI_Wait(request, status)`

How these statements are used was discussed in the introduction to message
passing, but for as a refresher will be repeated here.

- `Isend` - spawns thread to continually check if the receiver is ready to receiver data.
- `Irecv` - spawns thread to continually check if the sender is ready to
send data.
- `Wait` - forces the main process to wait until the corresponding send/receiver
is complete.

## Detailed Syntax

### The Four Asynchronous Sends

There are four ways of performing an asynchronous send. They symmetry to the
four methods of performing a synchronous send. They are:

- `MPI_Isend` - complete when data is copied to a buffer or receive is complete.
- `MPI_Ibsend` - complete when data is copied to a buffer.
- `MPI_Issend` - complete when receive posted and data copied.
<!-- What does this mean exactly? -->
- `MPI_Irsend` - complete when data is copied.

<!-- So is the only difference between Bsend and Ibsend, that Ibsend spawns a
thread to cover the transfer of the data to the buffer?  -->

### Term Definitions

The opening section of this module discussed the three basic syntactic
expression of asynchronous communication. However, arguments to these function
call were not explained. This section provides meaning and context to what those
argument mean.

| name   | Description | Example |
|:------:|:------------|---------|
| buffer | Address of the data. | `&array[0]` Address to `array`|
| count  | Number of elements.  | `5` Five elements of  `array` |
| datatype | DataType | `MPI_INT` (for other type see below) |
| dest/source | Rank to/from where the data is going/coming | `1` |
| tag | Message identifier | `0` Integer to distinguish multiple incoming. |
| communicator | Channel of communication | `MPI_COMM_WORLD` |
| request | Links `Isend`/`Irecv` with the correct `Wait` | See request section |
| status | Message information | See status section |

### Tag

This section contains more information about hte `tag` argument. Messages
with the same tag are guaranteed to be received in the order in which they are
sent. Many programmers do not use tags, and instead wil always use `tag=0` on
all of their messages.

### Request

This section contains more information about the `request` argument.

### Status

This section contains more information about the `status` argument. `status`
is a `structure` which contains three pieces of information, they are:

- `MPI_SOURCE` - Contains the source information (i.e., where is this data
coming from).
- `MPI_TAG` - Contains the integer tag.
- `MPI_ERROR` - Contains the resulting error code for the message.

This information can be helpful with debugging a code in development, however
this information is redundant in a production code. To remove the need to
allocate additional resources for this information in production codes, `MPI`
provided `MPI_STATUS_IGNORE` which can be used in lieu of `status`.

#### Wildcards

A related use case for status is will the use of `wildcards`. `Wildcards`
include `MPI_ANY_SOURCE` which accepts any message from any source and
`MPI_ANY_TAG` which accepts messages from any tag. These can be useful for when
you anticipate multiple messages from many sources and many tags. Once you have
received the information you can then use `status` to figure out the relevant
details.

Another wildcard is `MPI_PROC_NULL` which can be useful for more expressive
code. For example, say that each process passing its information to the right,
from the first process (farthest to the left) to the last (farthest to the right
). Since the last process has no where to pass its information, rather than
using control flow method to stop the sends, you can instead use `MPI_PROC_NULL`
which simply and unambiguously shows that the process is not meant to pass its
information along.

### Received Message Count

`MPI` provided `MPI_Get_count` for some reason ?

<!-- What is MPI_Get_count used for? -->

The full syntax is

`int err=MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count)`


