# Trapezium-Distributed
Exercise proposed on a Distributed systems class. Given an integral, it's interval and the number of clients, this program calculates the result of the definite integral using the number of clients as number of trapezoids. As you might expect, the more clients you run, more accurate the result is.

This program was coded in two ways: Using C and Sockets and using MPI (message passing interface).
Both work the same way and are stand-alone versions. You can find the complete program and instructions in their respective folders.

## Method
Each client will calculate the area of a given trapezium and send it back to the server, where the final result will be calculated.

Each client receives a message from the server with the first X value (start of it's trapezium interval), send a message asking the server for the second X value, receives it and start to compute the area.
With the x1 and x2, each client computes it's f(x1) and f(x2), and then the trapezium area. 
Once all the areas have been received by the server, it makes the sum of them, obtaining the approximate "under the curve" area.

The given info is:
- f(x) = sqrt(100²-x²), on the [0, 100] interval.
- Number of clients: 1, 2, 4 or 10

# MPI made with libmpich-dev (more details inside the mpi folder)
# C version MUST be compiled with gcc-7 (more details inside the c folder)

