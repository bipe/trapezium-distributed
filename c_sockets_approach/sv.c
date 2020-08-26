#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // defines socket operations
#include <sys/types.h>
#include <netinet/in.h> // defines IP protocols (constants)
#include <netdb.h> // database for IP addresses translations
#include <arpa/inet.h> // to convert host addresses
#include <errno.h> // used for error messages
#include<time.h>


int main(int argc, char *argv[ ]){
	int sock, bytes_recv, true = 1;
	char send_data [1024] , recv_data[1024];
	int sin_size;
	int n_clients = strtol(argv[1], NULL, 10); //n_clients : numero de clients que vão se conectar 1,2,4 ou 10, aqui transformado de string para int
	int connected[n_clients]; //conectados agora é um vetor com o numero de clients
	struct sockaddr_in server_addr, *client_addr[n_clients];


	//Checa se o numero de clients está de acordo com a especificação do trabalho (outros valores poderiam trazer aproximações grandes na integral)
	if(n_clients != 1 && n_clients !=2 && n_clients != 4 && n_clients !=10){
		printf("\nIt only works with 1,2,4 or 10 clients!\n");
		return 0;
	}

	//variáveis de medição de tempo
	struct timespec start, finish; 
	double elapsed;

	// Creates a socket and checks for success in that operation
	 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket creation Error");
		exit(1);
	 }
	// Adjusts some options for the socket just created
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,&true,sizeof(int)) == -1) {
		perror("Setsockopt Error");
		exit(1);
	}

	// Initialize socket parameters. Note the port address (5000)
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000); // host-endian to network-endian
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8); 

	// Links the logical socket to the actual port in transport layer
	if (bind(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) {
		perror("Unable to bind Error");
		exit(1);
	}
	// Defines the size of the input queue (5)
	if (listen(sock, 5) == -1) {
		perror("Listen caused an error due to queue size");
		exit(1);
	}
	printf("\nTCPServer Waiting for clients on port 5000");
	fflush(stdout);

	sin_size = sizeof(struct sockaddr_in);

	int tam_subintervalo = 100/n_clients; //divide o intervalo da integral em subintervalos (teremos 1,2,4 ou 10)
	int subintervalo = 0; //o primeiro subintervalo é de 0 até tam_subintervalo


	//primeiro, aceita conexões de todos os clients
	for(int i=0; i<n_clients;i++){
		connected[i] = accept(sock, (struct sockaddr *)&client_addr[i], &sin_size);
	}

	clock_gettime(CLOCK_MONOTONIC, &start); //Começa a contar o tempo

	//outro laço para mandar os dados que cada client precisa
	for(int i=0; i<n_clients;i++){
		sprintf(send_data, "%d", subintervalo); //transforma o int tam_subintervalo para string send_data na base decimal
		send(connected[i], send_data, strlen(send_data), 0); //envia valor a (começo do subintervalo)
		fflush(stdout);

		//recebe requisição do valor B do intervalo
		bytes_recv = recv(connected[i], recv_data,1024,0);
		recv_data[bytes_recv] = '\0';
		printf("\nClient %d says : %s \n", i+1, recv_data); //apenas estético para mostrar msg recebida pelos clients

		subintervalo += tam_subintervalo;

		sprintf(send_data, "%d", subintervalo);//transforma o int subintervalo para string send_data na base decimal
		send(connected[i], send_data, strlen(send_data), 0); //envia valor b (fim do subintervalo)
		fflush(stdout);
	}

	double area_client[n_clients]; //vetor onde recebemos as areas dos i trapezios
	double resultado=0;

	//recebe dos i clients o valor da area de cada trapezio
	for(int i=0; i<n_clients;i++){
		bytes_recv=recv(connected[i], recv_data,1024,0);
		recv_data[bytes_recv] = '\0';
		sscanf(recv_data, "%lf", &area_client[i]);
		printf("\nArea do trapezio do client %d = %lf ", i+1, area_client[i]);
		resultado += area_client[i];
	}
	close(sock); //fecha o canal


	clock_gettime(CLOCK_MONOTONIC, &finish); //termina de contar o tempo
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	//Intervalo de discretização (arredondamento)
	int casas;
	printf("\nQuantas casas decimais no resultado? (4,5 ou 6)? ");
	scanf("%d", &casas);

	if(casas == 4) printf("\nResultado da integral (soma da area dos trapezios): %.4f\n", resultado);
	else if(casas == 5) printf("\nResultado da integral (soma da area dos trapezios): %.5f\n", resultado);
	else printf("\nResultado da integral (soma da area dos trapezios): %lf\n", resultado);

	printf("\nTempo decorrido: %lf\n", elapsed);

	return 0;
} 