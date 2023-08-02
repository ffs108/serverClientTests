/*
	C ECHO client example using sockets
*/
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

#define TCP_PORT  8080


int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in serverInfo;
	char message[1000] , reply[2000];
	
	// Create the socket as a string socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");  // connect to the localhost
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons( TCP_PORT );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&serverInfo , sizeof(serverInfo)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	printf("Connected to server.\n");
	
	//keep communicating with server
	while (1)
	{
		printf("Enter message : ");
		fgets(message, sizeof(message)-1, stdin);
        message[strlen(message)-1] = 0;
		
		//Send some data
		if (send(sock, message, strlen(message)+1, 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		
		//Receive a reply from the server
		if (recv(sock, reply, sizeof(reply), 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		printf("Server's reply: %s\n", reply);
	}
	
	close(sock);
	return 0;
}