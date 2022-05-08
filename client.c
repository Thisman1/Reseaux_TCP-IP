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
#define BUFFER_SIZE 100
//On définit le port
const char *PORT = "8080";

//print par défaut
void reset () {
  printf("\033[0m");
}

//print en vert
void green () {
    printf("\033[0;32m");
}

//Function to reassemble and print the message from the server
void print_reassemble_message(char *message) {
    //On check si le message est trop long
    if (strlen(message) > BUFFER_SIZE) {
        //On reassemble le message
        char *reassembled_message = malloc(BUFFER_SIZE);
        int i = 0;
        int j = 0;
        while (i < strlen(message)) {
            //On copie le fragment
            strncpy(reassembled_message + j, message + i, BUFFER_SIZE);
            //On incremente l'index
            i += BUFFER_SIZE;
            j += BUFFER_SIZE;
        }
        //On affiche le message
        printf("Server: %s\n", reassembled_message);
        //On libère le message
        free(reassembled_message);
    } else {
        //Si le message n'est pas trop long, on affiche le message
        printf("Server: %s\n", message);
    }
}

//Function to fragment the message if it is too long and send it to the server and the server will reassemble it
void send_fragmented_message(int server_fd, char *message) {
    int i = 0;
    int j = 0;

    //On check si le message est trop long
    if (strlen(message) > BUFFER_SIZE) {
        //On fragmente le message
        char *fragment = malloc(BUFFER_SIZE);
        while (i < strlen(message)) {
            //On copie le fragment
            strncpy(fragment, message + i, BUFFER_SIZE);
            //On envoie le fragment
            send(server_fd, fragment, BUFFER_SIZE, 0);
            //On incremente l'index
            i += BUFFER_SIZE;
        }
        //On libère le fragment
        free(fragment);
    } else {
        //On envoie le message si on a pas besoin de le fragmenter
        send(server_fd, message, strlen(message), 0);
    }
}



//Function to chat between client and server
void chat(int sockfd)
{
    char msg1 [BUFFER_SIZE];
    char msg2 [BUFFER_SIZE];
    int read_size;
    while(1)
    {
        //Si le message est "Exit" le client va fermer la connexion
        if(strncmp(msg1, "Exit", 4) == 0)
        {
            puts("Client is closing\n");
            break;
        }
        //Read the message sent by the server
        read_size = recv(sockfd, msg2, BUFFER_SIZE, 0);
        if(strncmp(msg2, "Exit", 4) == 0)
        {
            puts("Server is closing\n");
            break;
        }
        //Si le recv renvoie 0 c'est que le server a fermé la connexion
        if(read_size == 0)
        {
            puts("Server disconnected\n");
            //utilisation de fflush pour effacer le buffer
            fflush(stdout);
            break;
        }
        //Si le recv renvoie -1 c'est que quelque chose s'est mal passé
         else if(read_size == -1)
        {
            perror("Reception message failed\n");
            break;
        }
        //On affiche le message reçu en utilisant la fonction print_reassemble_message
        //print_reassemble_message(msg2);
        printf("Server: %s", msg2);
        fflush(stdout);
        //fragmentation(sockfd);
        //Enter the message to be sent
        printf("Enter the message: ");
        fgets(msg1, BUFFER_SIZE, stdin);
        //Send the message to the server
        //send_fragmented_message(sockfd, msg1);
        write(sockfd, msg1, strlen(msg1)+1); //+1 pour le \0

    }
}


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
