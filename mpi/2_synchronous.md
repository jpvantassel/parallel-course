# Syncrhonous Commucication

> Joseph P. Vantassel, The University of Texas at Austin

[![License](https://img.shields.io/badge/license-CC--By--SA--4.0-brightgreen.svg)](https://github.com/jpvantassel/parallel-course/blob/master/LICENSE.md)

- Ssend - Synchronous communication, beware of deadlocks.
- Bsend - Buffered communication, beware of buffer size and the size of
messages currently inside the buffer. If the buffer overflows the pass will
fail.
- Rsend - Ready communication, beware that receive is posted prior to send, this
is not commonly used.
