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


int main(int argc, char *argv[ ]){
	int sock, bytes_recv, true = 1;
	char send_data [1024] , recv_data[1024];
	struct sockaddr_in server_addr, client_addr;
	int sin_size;

	int n_slaves = strtol(argv[1], NULL, 10); //n_slaves : numero de escravos que vão se conectar

	//int connected[n_slaves]; //conectados agora é um vetor com o numero de escravos
	int connected;

	// Creates a socket and checks for success in that operation
	 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
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
		perror("Unable to bind");
		exit(1);
	}
	// Defines the size of the input queue (5)
	if (listen(sock, 5) == -1) {
		perror("Listen caused an error due to queue size");
		exit(1);
	}
	printf("\nTCPServer Waiting for client on port 5000");
	fflush(stdout);

	while(1){
		sin_size = sizeof(struct sockaddr_in);
		connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
		printf("\nGot connection from (%s , %d)", inet_ntoa(client_addr.sin_addr),
		ntohs(client_addr.sin_port));

		while (1){ 
			printf ("\n SEND (q or Q to quit) : ");
			gets(send_data);

			if (strcmp(send_data,"q") ==0 || strcmp(send_data,"Q") ==0) {
				send(connected, send_data, strlen(send_data), 0);
				close(connected);
				break;
			}

			else send(connected, send_data, strlen(send_data), 0);
			bytes_recv = recv(connected, recv_data,1024,0);
			recv_data[bytes_recv] = '\0';
			if (strcmp(recv_data,"q") ==0 || strcmp(recv_data,"Q") ==0){
				close(connected);
				break;
			}

			else printf("\n RECEIVED DATA = %s " , recv_data);
			fflush(stdout);
		}
	}
	close(sock);
	return 0;
} 