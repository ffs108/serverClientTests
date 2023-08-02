
/*****************************************************
 *
 *  CSCV352 - server.c
 *
 *  Student:  Francisco Figueroa
 *
******************************************************/
#include <stdio.h>
#include <string.h>	
#include <unistd.h>	
#include <sys/socket.h>
#include <arpa/inet.h>	
#include <errno.h>
#include <fcntl.h>

///////////////////////////////////////////////////////
//                CONSTANTS
///////////////////////////////////////////////////////
#define LISTENER_PORT  8080


/*****************************************************
 *
 *  Function:  main()
 *
 *  Parameters:
 *
 *      argc - main parameter for argument count
 *      argv - main parameter for argument values
 *
******************************************************/
int main(int argc , char *argv[])
{
	int listener;
    int connectedSocket;
    int addrLen;
    int bytesRead;
	struct sockaddr_in serverAddr , clientAddr;
	char data[2000];
    struct sockaddr_in* pClientAddr;
    char clientIp[32]; 
	
	// create the listening socket
	listener = socket(AF_INET , SOCK_STREAM , 0);
	if (listener == -1)
	{
		printf("Error creating socket: %d\n", errno);
	}

	serverAddr.sin_family = AF_INET;              // IP V4 addresses
	serverAddr.sin_addr.s_addr = INADDR_ANY;      // don't bind to a specific address
	serverAddr.sin_port = htons( LISTENER_PORT ); // set the listening port
	
    // bind the address information to the listener socket
	if( bind(listener,(struct sockaddr *)&serverAddr , sizeof(serverAddr)) < 0)
	{
		perror("Error binding socket address information.\n");
		return 1;
	}
	
	// start the listening process
	listen(listener, 3);
	
	printf("Waiting for incoming connections...\n");

    // initialize the size variable to the size of the sockaddr_in struct.
	addrLen = sizeof(struct sockaddr_in);	

	//accept connection from an incoming client
	//return here
	connectedSocket = accept(listener, (struct sockaddr *)&clientAddr, (socklen_t*)&addrLen);
	while (1)
	{
		if (connectedSocket < 0)
		{
			printf("accept returned: %d, %s\n", connectedSocket, strerror(errno));
			return 1;
		}

		pClientAddr = (struct sockaddr_in*)&clientAddr;
		inet_ntop(AF_INET, &(pClientAddr->sin_addr), clientIp, INET_ADDRSTRLEN);
		printf("Connection accepted from: %s\n", clientIp);
		
		// receive data from the client
		while ((bytesRead = recv(connectedSocket, data, sizeof(data), 0)) > 0 )
		{
			// echo the string back to client
			write(connectedSocket, data, strlen(data));
		}
		connectedSocket = accept(listener, (struct sockaddr *)&clientAddr, (socklen_t*)&addrLen);
	}
	if (bytesRead == 0)
	{
		printf("Recv returned 0, client disconnected.\n");
		fflush(stdout);
	}
	else if (bytesRead == -1)
	{
		perror("Recv function return an error\n");
	}

	


	
	return 0;
}

void SetBlockingMode(int fd)
{
    // where socketfd is the socket you want to make non-blocking
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1)
    {
		perror("Error setting socket to non-blocking mode.\n");
    }

}