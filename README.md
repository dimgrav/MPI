# MPI
MPI = Message Passing Interface

MPI is a specification for the developers and users of message passing libraries. By itself, it is NOT a library - but rather the specification of what such a library should be.

MPI primarily addresses the message-passing parallel programming model: data is moved from the address space of one process to that of another process through cooperative operations on each process.

source: https://computing.llnl.gov/tutorials/mpi/#What

Used in Distributed and Parallel Systems.

The objective of this repository is to demonstrate the difference in calculation time between serial and parallel algorithms, measure performance and scalability and find the turning point, after which the communication cost between nodes exceeds the computational benefit.
