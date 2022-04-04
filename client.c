#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
//On définit le nombre de clients maximum
#define MAX_CLIENTS 10
//On définit la taille du buffer
#define BUFFER_SIZE 1024
//On définit le port
const char *PORT = "8080";



void chat(int sockfd)
{
    char msg1 [BUFFER_SIZE];
    char msg2 [BUFFER_SIZE];
    int read_size;
    send(sockfd, "Welcome to the chat room!\n", 100, 0);
    while(1)
    {
        //Si le message est "Exit" le client va fermer la connexion
        if(strncmp(msg1, "Exit", 4) == 0)
        {
            puts("Client is closing");
            break;
        }
        //Read the message sent by the server
        read_size = recv(sockfd, msg2, BUFFER_SIZE, 0);
        if(strncmp(msg2, "Exit", 4) == 0)
        {
            puts("Server is closing");
            break;
        }
        //Si le recv renvoie 0 c'est que le server a fermé la connexion
        if(read_size == 0)
        {
            puts("Server disconnected");
            //utilisation de fflush pour effacer le buffer
            fflush(stdout);
            break;
        }
        //Si le recv renvoie -1 c'est que quelque chose s'est mal passé
         else if(read_size == -1)
        {
            perror("Reception message failed");
            break;
        }
        //On affiche le message reçu
        printf("Server: %s\n", msg2);
        fflush(stdout);
        //Enter the message to be sent
        printf("Enter the message: ");
        fgets(msg1, BUFFER_SIZE, stdin);
        //Send the message to the server
        write(sockfd, msg1, strlen(msg1)+1); //+1 pour le \0

    }
}
//print par défaut
void reset () {
  printf("\033[0m");
}

//print en vert
void green () {
    printf("\033[0;32m");
}

//Function to chat between client and server


//Main function for the client side
int main(int argc , char *argv[])
{
    int client_fd;  
    struct addrinfo address_ip;
    struct addrinfo *result, *client;
    int res;



	if (argc < 2)
		{
			fprintf (stderr, "ERROR !!! not enough input\n");
			exit (0);
		}

 /* Obtain address(es) matching host/port */

           memset(&address_ip, 0, sizeof(address_ip));
           address_ip.ai_family = AF_INET;    /* Unsecific allow IPv4 or IPv6 only IPv4 here */
           address_ip.ai_socktype = SOCK_STREAM; /* Type of socket, here we have a TCP socket */
           address_ip.ai_protocol = IPPROTO_TCP;  //0
             /* Any protocol */

           res = getaddrinfo(argv[1], PORT, &address_ip, &result);
           if (res != 0) 
			   {
				   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
				   exit(EXIT_FAILURE);
			   }
			   
	for (client = result; client != NULL; client = client->ai_next)
	{
		if ((client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		{
			continue;
		}
		
			green();
			printf("Socket successfully created\n");
			reset();

					//connect the socket to the server
	
					
				if ( connect(client_fd, client->ai_addr, client->ai_addrlen) != -1 )
				{
					
					break;
					close(client_fd);
				}
	}
	if (client == NULL)
				{
					perror ("Failed to create and connected !!! \n");
					exit(EXIT_FAILURE);
				}
				
		//printf ("Client socket created and connected \n");
    /*char * message = " Connexion ";
	if (send (client_fd,message,strlen(message),0) == -1 )
	{
		perror ("Failed !!!!! \n");
		exit(EXIT_FAILURE);
	}*/
	else
	{
		printf ("Client socket created and connected \n");
	}

    //Function for chat
    chat(client_fd);
    //Close the socket
    freeaddrinfo(result);
    

return 0;
}
