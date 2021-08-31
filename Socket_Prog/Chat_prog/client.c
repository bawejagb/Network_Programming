/*
Made By : Gaurav Baweja
Cmd line arguments: Server_IP_Address port_number [range (1025-65500)]
Ends: when server type "Bye" text
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[255];
	if(argc < 3){
		fprintf(stderr, "usage %s hostname port. \n",argv[0]);
		exit(1);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("Error in opening socket!\n");
	server = gethostbyname(argv[1]);
	if(server == NULL)
		fprintf(stderr, "Error, no such host");
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family  = AF_INET;
	bcopy((char*) server->h_addr, (char*) & serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0)
		error("Connection Failed!\n");
	strcpy(buffer, "Connection Established!");
	n = write(sockfd, buffer, strlen(buffer));
	if(n < 0)
		error("Error on writing!\n");
	n = read(sockfd, buffer, 255);
	if( n < 0)
		error("Error on reading!\n");
	printf("%s\n>> ", buffer);
	int status = strncmp("Connection Established!", buffer, 23);
	while(status == 0){
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if(n < 0)
			error("Error on writing!\n");
		bzero(buffer, 255);
		n = read(sockfd, buffer, 255);
		if( n < 0)
			error("Error on reading!\n");
		printf("Server : %s>> ", buffer);
		int t = strncmp("Bye", buffer, 3);
		if(t == 0)
			break;
	}
	close(sockfd);
	return 0;
}