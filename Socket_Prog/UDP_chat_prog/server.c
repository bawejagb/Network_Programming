/*
Made By : Gaurav Baweja
Cmd line arguments: port_number [range (1025-65500)]
Program Ends: when server type "bye" text
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

void program(char buffer[255], int sockfd){
	int n, len;
	struct sockaddr_in cliaddr;
	len = sizeof(cliaddr);
	while(1){
		bzero(buffer,255);
		n = recvfrom(sockfd, (char *)buffer, 255, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
		if(n < 0)
			error("Error on reading!\n");
		printf("Client: %s>> ", buffer);
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = sendto(sockfd, buffer, strlen(buffer), 
        	MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
		if(n < 0)
			error("Error on writing");
		int t = strncmp("bye", buffer, 3);
		if(t == 0)
			break;
	}
}

int main(int argc, char * argv[]){
	if(argc < 2){
		fprintf(stderr, "Port no. not provided. Program terminated!\n");
		exit(1);
	}
	int sockfd, portno, n;
	char buffer[255];
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
		error("Error opening socket.\n");
	bzero((char *) & serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		error("Binding failed!\n");
	program(buffer, sockfd);
	close(sockfd);
	return 0;
}