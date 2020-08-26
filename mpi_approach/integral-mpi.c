//MPI_Send: parametros em ordem: endereço dos dados a enviar, nº de elementos que serao enviados, tipo do dado enviado, qual processo recebe, tag (0), dentro do grupo comm_world.
//MPI_Recv: endereço dos dados, nº elementos recebidos, tipo, processo fonte esperado, tag, grupo, status (infos extras)

#include <stdio.h> 
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){
	int casas = strtol(argv[1], NULL, 10); //intervalo de discretização como argumento (4,5 ou 6). Caso insira valor invalido, sera usado 6.

	int size, myRank; //size = tamanho do grupo = numero de processos, myRank = id do processo (0 é server, demais são clients)
	MPI_Status st; //status, não usado nesse exemplo

	int n_processos; //numero de processos (clients+servidor)

	//variáveis de medição de tempo
	struct timespec start, finish; 
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start); //Começa a contar o tempo


	//Inicialização do MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if(size != 2 && size !=3 && size != 5 && size !=11){ //checa se o número de processos está de acordo com o proposto no exercício (1,2,4 ou 10 clients + server)
		printf("\nIt only works with 2,3,5 or 11 processes! (clients+server)\n");
		return 0;
	}

	int tam_subintervalo = 100/(size-1); //divide o intervalo da integral em subintervalos (teremos 1,2,4 ou 10)
	int subintervalo = 0; //o primeiro subintervalo é de 0 até tam_subintervalo

	//Variaveis para calculo da integral nos processos dos clients
	int a, b;
	float h;
	double fa, fb, area_trapezio;
	double soma_areas_recebidas = 0;


	printf("INICIO: Processo %d de %d\n", myRank, size);
	fflush(stdout);

	if(myRank == 0){ //Rank 0 é o rank do MESTRE

		for(n_processos = 1; n_processos < size; n_processos++){ //começa em 1 pois o 0 é o processo do servidor
			MPI_Send(&subintervalo, 1, MPI_INT, n_processos, 0, MPI_COMM_WORLD);
			
			printf("SERVER diz: enviei valor %d pro client %d\n", subintervalo, n_processos);
			fflush(stdout);

			subintervalo+= tam_subintervalo; //manda agora o fim do subintervalo do processo

			MPI_Send(&subintervalo, 1, MPI_INT, n_processos, 0, MPI_COMM_WORLD);
			printf("SERVER diz: enviei valor %d pro client %d\n", subintervalo, n_processos);
			fflush(stdout);

			//Recebimento dos resultados:
			MPI_Recv(&area_trapezio, 1, MPI_DOUBLE, n_processos, 0, MPI_COMM_WORLD, &st);
			printf("SERVER diz: Area do client %d recebida: %lf\n", n_processos, area_trapezio);
			soma_areas_recebidas += area_trapezio;
		}

		//ao fim do laço de todos os processos, exibe resultado (soma das areas recebidas)

		//Intervalo de discretização definido como parametrode execução (4,5 ou 6). Valores invalidos imprimem 6 casas após a virgula.
		if(casas == 4) printf("*\n*FIM: Resultado da integral (soma da area dos trapezios): %.4f\n*\n", soma_areas_recebidas);
		else if(casas == 5) printf("*\n*FIM: Resultado da integral (soma da area dos trapezios): %.5f\n*\n", soma_areas_recebidas);
		else printf("*\n*FIM: Resultado da integral (soma da area dos trapezios): %lf\n*\n", soma_areas_recebidas);

	}

	else{ //outros rankings são CLIENTS

		//Recebe o COMEÇO do subintervalo do MESTRE:
		MPI_Recv(&subintervalo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &st); 
		printf("Client %d - Valor a recebido: %d\n", myRank, subintervalo);
		fflush(stdout);
		a = subintervalo;

		//Recebe o FIM do subintervalo do MESTRE:
		MPI_Recv(&subintervalo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &st); 
		printf("Client %d - Valor b recebido: %d\n", myRank, subintervalo);
		fflush(stdout);
		b = subintervalo;

		h = b-a; //calcula tamanho do subintervalo recebido
		fa = sqrt(pow(100,2) - pow(a,2)); //calcula f(a) - base maior do trapezio
		fb = sqrt(pow(100,2) - pow(b,2)); //calcula f(b) - base menor do trapezio

		printf("f(%d): %lf\n", a, fa);
		printf("f(%d): %lf\n", b, fb);

		area_trapezio = (h/2)*(fa+fb); //calcula area do trapezio que vai de x=a ate x=b
		printf("Client %d - Area do trapezio designado: %lf \n", myRank, area_trapezio);

		MPI_Send(&area_trapezio, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); //envia a area para o mestre
		printf("Client %d - Resultado enviado ao servidor: %lf\n", myRank, area_trapezio);
		fflush(stdout);
	}

	MPI_Finalize();

	clock_gettime(CLOCK_MONOTONIC, &finish); //termina de contar o tempo


	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("\nTempo decorrido: %lf\n", elapsed);
	return 0;
}
