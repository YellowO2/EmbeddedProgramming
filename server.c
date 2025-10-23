#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void error(char *m)
{
	perror(m);
}
	
void *handle_socket(int *newsockfd)
{
	char buffer[256];
	int n;
	memset(buffer, 0, 256);
	n = read(*newsockfd,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("Message received: %s\n",buffer);
	
	int num = 5 * atoi(buffer);
	
	sprintf(buffer, "%d", num);
	
    n = write(*newsockfd,buffer,255);
    if (n < 0)
        error("ERROR writing back to socket");
}

int setup_server(int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Prepare the sockaddr_in structure
    memset((char*) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Bind the host address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // Start listening for the clients
    listen(sockfd, 5); // A backlog of 5 is common
    
    return sockfd;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, port;
    struct sockaddr_in cli_addr;
    socklen_t clilen; // Use socklen_t for portability

    // Check for port number argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    // 1. Set up the server and get the listening socket
    sockfd = setup_server(port);
    printf("Server is listening on port %d. Waiting for connections...\n", port);
    
    clilen = sizeof(cli_addr);

    // 2. This is the main server loop. Use while(1) for a real server.
    //    For your lab, we can use a for-loop to serve a few clients and then stop.
    int i = 0;
    for (i = 0; i < 3; i++) { // serve 3 clients
        // Wait for a client to connect. This call BLOCKS until a client connects.
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        
        printf("Connection accepted from client #%d\n", i + 1);

        pthread_t client_thread;
        // Allocate memory for the socket descriptor to pass to the thread safely
        int *new_sock = malloc(sizeof(int));
        *new_sock = newsockfd;
        
        // Create a new thread to handle this client
        if (pthread_create(&client_thread, NULL, handle_socket, (void*) new_sock) < 0) {
            error("ERROR creating thread");
        }
        // IMPORTANT: We DO NOT call pthread_join() here.
        // We want the main loop to continue and accept the next client immediately.
    }
    
    // In a real server with a while(1) loop, this part is never reached.
    // But for our for-loop example, we close the main listening socket when done.
    printf("Server finished serving clients. Shutting down.\n");
    close(sockfd);
    
    return 0;
}