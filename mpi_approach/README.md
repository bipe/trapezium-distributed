# MPI approach 
For more information about this algorithm, read the main README.md (on the root of this repository).
MPI approach to solve a definite integral using the trapezium rule. Proposed excercise in Distributed Systems class where, given a f(x), the result must be calculated using mpi.
Code is in PTBR, it may (or may not) have a future english version.

The given info is:
- f(x) = sqrt(100²-x²), on the [0, 100] interval.
- Number of clients: 1, 2, 4 or 10

# Made with libmpich-dev

## Instructions (for more details check instructions.txt)

Compile:
$ mpicc file.c -lm -o executable

Execute:
$ mpirun -n X ./executable Y

Where X is number of nodes (clients + 1 server) (values must be 2, 3, 5 or 11)   
And Y is the number of decimals of the result (4, 5 or 6)

### Warning: For openMPI instructions, check instructions.txt


## Método

Método dos trapézios:
- A área de um trapézio é (B+b). h/2. Base maior é f(xa) e base menor é f(xb), onde xa e xb são os valores do começo e fim do intervalo. A altura h é xb-xa.
-A área a ser calculada é dividida em n trapézios, sendo n o número de clients (1,2,4 ou 10), e cada client calcula a área de seu trapézio e depois a envia para o servidor, que soma as areas dos trapézios recebidos obtendo a área aproximada sob o gráfico.

### Lookup values for conference

f(0) = 100  
f(25) = 25sqrt(15) = 96,82458  
f(50) = 50sqrt(3) = 86,60254  
f(75) = 25sqrt(7) = 66,14378  
f(100) = 0  
