#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> // defines IP standard protocols
#include <netdb.h> // defines sockaddr_in structure
#include <errno.h> // used for error messages



int main(int argc, char *argv[ ]){
	
	int sock, bytes_recv;
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr; 

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

	while(1){
		bytes_recv=recv(sock,recv_data,1024,0);
		recv_data[bytes_recv] = '\0';

		if (strcmp(recv_data,"q") == 0 || strcmp(recv_data,"Q") == 0){
			close(sock);
	 		break;
	 	}
	 	else printf("\nReceived data = %s " , recv_data);

		printf("\nSEND (q or Q to quit) : ");
		gets(send_data);

		if (strcmp(send_data,"q") != 0 && strcmp(send_data,"Q") != 0) send(sock,send_data,strlen(send_data), 0); //parada

		else{
			send(sock,send_data,strlen(send_data), 0);
		 	close(sock);
		 	break;
		}
	}
	return 0;
}

