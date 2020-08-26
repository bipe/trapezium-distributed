#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> // defines IP standard protocols
#include <netdb.h> // defines sockaddr_in structure
#include <errno.h> // used for error messages
#include <math.h>
#include<time.h>


int main(int argc, char *argv[ ]){
	
	int sock, bytes_recv;
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr; 
	char *ptr; //usado para converter a string de recv_data para int

	//variáveis de medição de tempo
	struct timespec start, finish; 
	double elapsed;


	host = gethostbyname(argv[1]);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

	if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1){
		perror("Connect");
		exit(1);
	}

	printf("\nAguardando valores do servidor...");

	//client recebe o valor a (começo do subintervalo)
	bytes_recv=recv(sock,recv_data,1024,0);
	recv_data[bytes_recv] = '\0';
	int a = strtol(recv_data, &ptr, 10); //transforma string recebida em inteiro

	clock_gettime(CLOCK_MONOTONIC, &start); //Começa a contar o tempo

	//pede para que o servidor mande o valor b (fim do subintervalo)
	memcpy(send_data,"requesting b", 13); //Copia segundo parametro para a string send_data. ultimo parametro é o tamanho da string+1 (/0)
	send(sock,send_data,strlen(send_data), 0);

	//recebe do servidor o valor b
	bytes_recv=recv(sock,recv_data,1024,0);
	recv_data[bytes_recv] = '\0';
	int b = strtol(recv_data, &ptr, 10); //transforma string recebida em inteiro

	printf("\n\nValor a = %d\nValor b = %d\n", a, b);
	
	//A área de um trapézio é (B+b)(h/2)

	float h = b-a; //tamanho do subintervalo

	double fa = sqrt(pow(100,2) - pow(a,2)); //calcula f(a) - base maior do trapezio
	double fb = sqrt(pow(100,2) - pow(b,2)); //calcula f(b) - base menor do trapezio

	printf("\nf(%d): %lf\n", a, fa);
	printf("f(%d): %lf\n", b, fb);

	double area_trapezio = (h/2)*(fa+fb); //calcula area do trapezio que vai de x=a ate x=b

	printf("\nArea do trapezio designado: %lf \n", area_trapezio);

	sprintf(send_data, "%lf", area_trapezio); //coloca o double area_trapezio na string send_data
	send(sock,send_data,strlen(send_data), 0);
	close(sock);

	clock_gettime(CLOCK_MONOTONIC, &finish); //termina de contar o tempo


	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("\nTempo decorrido: %lf\n", elapsed);

	return 0;
}

