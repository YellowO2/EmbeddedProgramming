#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void error(char *m)
{
       	perror(m);
	exit(0);
}
int main(int argc, char *argv[ ])
{ 
	int sockfd, port, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	if (argc < 3)
	       	error("fuck u type properly its: client [hostname] [port]\n");
	
	port = atoi(argv[2]);
	
	// empty socket: TCP IPv4 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	       	error("ERROR opening socket");
	
	server = gethostbyname(argv[1]);
	if (server == NULL) 
		error("ERROR, no such host\n");
	
	// start with a clean address structure
	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET; // internet socket
	bcopy((char *)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
	
	serv_addr.sin_port = htons(port);
	
	if (connect(sockfd,(const struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");
	
	printf("Please enter a number: ");
	fgets(buffer,255,stdin);
    
    // Send the number to the server
    printf("Sending: %s", buffer);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) 
        error("ERROR writing to socket");

    // --- THIS IS THE IMPORTANT PART ---
    // WIPE THE BUFFER CLEAN before waiting for the reply.
    memset(buffer, 0, 256); 

    // Now, read the reply from the server
    n = read(sockfd, buffer, 255);
    if (n < 0) 
        error("ERROR reading from socket");

    // Print whatever the server sent back
    printf("Server replied: %s\n", buffer);

    close(sockfd);
    return 0;
}