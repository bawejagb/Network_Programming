/*
Made By : Gaurav Baweja
Cmd line arguments: port_number [range (1025-65500)]
Program Ends: when server type "Bye" text
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}

void program(int status, char buffer[255], int newsockfd){
	int n;
	while(status == 0){
		bzero(buffer,255);
		n = read(newsockfd, buffer, 255);
		if(n < 0)
			error("Error on reading!\n");
		printf("Client: %s>> ", buffer);
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(newsockfd, buffer, strlen(buffer));
		if(n < 0)
			error("Error on writing");
		int t = strncmp("Bye", buffer, 3);
		if(t == 0)
			break;
	}
}

int main(int argc, char * argv[]){
	if(argc < 2){
		fprintf(stderr, "Port no. not provided. Program terminated!\n");
		exit(1);
	}
	int sockfd, newsockfd, portno, n;
	char buffer[255];
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("Error opening socket.\n");
	bzero((char *) & serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		error("Binding failed!\n");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr*) & cli_addr, &clilen);
	if(newsockfd < 0)
		error("Error on accept!\n");
	n = read(newsockfd, buffer, 255);
	if( n < 0)
		error("Error on reading!\n");
	printf("%s\n", buffer);
	strcpy(buffer, "Connection Established!");
	n = write(newsockfd, buffer, strlen(buffer));
	if(n < 0)
		error("Error on writing!\n");
	int status = strncmp("Connection Established!", buffer, 23);
	program(status, buffer, newsockfd);
	close(newsockfd);
	close(sockfd);
	return 0;
}
		