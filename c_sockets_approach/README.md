# C sockets approach 
For more information about this algorithm, read the main README.md (on the root of this repository).
Simple C sockets application to solve a definite integral using the trapezium rule. Proposed excercise in Distributed Systems class where, given a f(x), the result must be calculated using mpi.
Code is in PTBR, it may (or may not) have a future english version.

The given info is:
- f(x) = sqrt(100²-x²), on the [0, 100] interval.
- Number of clients: 1, 2, 4 or 10

## WARNING: MUST be compiled with gcc-7. Later versions don't work properly.

## Instructions

Compile:  
$ gcc-7 sv.c -o server  
$ gcc-7 cli.c -lm -o client  

Execute:  
$ ./server X
Where X is number of clients (values must be 1, 2, 4 or 12)   


$ ./client TARGET-IP
Where TARGET-IP is the server's ip (test with localhost if you want to run locally)


Where X is number of nodes (clients + 1 server) (values must be 2, 3, 5 or 11)   
And Y is the number of decimals of the result (4, 5 or 6)


## Lookup values for conference

f(0) = 100  
f(25) = 25sqrt(15) = 96,82458  
f(50) = 50sqrt(3) = 86,60254  
f(75) = 25sqrt(7) = 66,14378  
f(100) = 0  