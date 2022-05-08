#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//On définit le nombre de clients maximum
#define MAX_CLIENTS 10
//On définit la taille du buffer
#define BUFFER_SIZE 100
//On définit le port
const char *PORT = "8080";

int server_fd, new_socket;  
    struct addrinfo address_ip;
    struct addrinfo *result, *server;
    int res;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    pid_t client_pid;

//print par défaut
void reset () {
  printf("\033[0m");
}

//print en vert
void green () {
    printf("\033[0;32m");
}


//Function to fragment the message if it is too long and send it to the client and the client will reassemble it
void send_fragmented_message(int client_fd, char *message) {
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
            send(client_fd, fragment, BUFFER_SIZE, 0);
            //On incremente l'index
            i += BUFFER_SIZE;
        }
        //On libère le fragment
        free(fragment);
    } else {
        //On envoie le message si on a pas besoin de le fragmenter
        send(client_fd, message, strlen(message), 0);
    }
}

//Function to reassemble and print the message from the client
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
        printf("Client: %s\n", reassembled_message);
        //On libère le message
        free(reassembled_message);
    } else {
        //Sinon on affiche le message s'il n'a pas besoin d'être fragmenté
        printf("Client: %s\n", message);
    }
}



//Function to chat between client and server
void chat(int sockfd)
{
    char msg1 [BUFFER_SIZE];
    char msg2 [BUFFER_SIZE];
    int read_size;
    //Message de bienvenue
	send(sockfd, "Welcome to the chat room!\n", 25, 0);
    while(1)
    {
        //Si le message est "Exit" le server va fermer la connexion
        if(strncmp(msg2, "Exit", 4) == 0)
        {
            puts("Server is closing");
            break;
        }
        //Read the message sent by the server
        read_size = recv(sockfd, msg1, BUFFER_SIZE, 0);
        if (strncmp(msg1, "Exit", 4) == 0)
        {
            puts("Client is closing");
            printf("Disconnected from %s to : %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            break;
        }
        //Si le recv renvoie 0 c'est que le client a fermé la connexion
        if(read_size == 0)
        {
            puts("Client disconnected");
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
        //On affiche le message reçu en utilisant la fonction print_reassemble_message
        //print_reassemble_message(msg1);
        printf("Client: %s\n", msg1);
        fflush(stdout);
        //Enter the message to be sent
        printf("Enter the message: ");
        fgets(msg2, BUFFER_SIZE, stdin);
        //On envoie le message en vérifiant si il faut fragmenter le message
        //send_fragmented_message(sockfd, msg2);
        write(sockfd, msg2, strlen(msg2)+1);//+1 pour le \0


    }
}

//Main function
int main(int argc , char *argv[])
{
    
	if (argc < 2)
	{
		fprintf (stderr, "ERROR !!! not enough input\n");
		exit (0);
	}
    
    /* Obtain address(es) matching host/port */

    memset(&address_ip, 0, sizeof(address_ip)); //Ici on utilise memset pour initialiser la structure à 0
    address_ip.ai_family = AF_INET;    		/* Unspecific allow IPv4 or IPv6 ==> AF_UNSPEC */
    address_ip.ai_socktype = SOCK_STREAM; 	/* Type of socket, here we have a TCP socket */
    address_ip.ai_flags = AI_PASSIVE;
    address_ip.ai_protocol = 0;				//PPROTO_TCP;          /* Any protocol */

    res = getaddrinfo(NULL, PORT, &address_ip, &result);
    if (res != 0) 
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
		exit(EXIT_FAILURE);
	}
			   
	for (server = result; server != NULL; server = server->ai_next)
	{
		//Create the socket

		if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		{
			continue;
		}
		else
		{
			green();
			printf("Socket successfully created\n");
			reset();
		}
		int tr=1;

		// kill "Address already in use" error message
		if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) 
		{
			perror("setsockopt");
			exit(1);
		}
		if ( bind (server_fd, server->ai_addr, server->ai_addrlen) == 0)
        {
            green();
            printf("Socket successfully binded\n");
            reset();
            break;
        }
        else
        {
            close(server_fd);
            continue;
        }

	}
	if (server == NULL)
	{
        perror("\033[0;31m Failed to create and to bind the client socket.");
        exit(EXIT_FAILURE);
    }
    
    //Listening
    if(listen(server_fd, 10) == -1)// 10 est le nombre de clients maximum, longueur maximale de la file d'attente
    {
        perror("\033[0;31mListen failed !!!");
        reset();
        exit(EXIT_FAILURE);
    }
    else
    {
        green();
        printf("Server is listening\n");
        reset();
    }

//On attend une connexion tant que le server n'est pas fermé, si le server est fermé on quitte le programme
    while(1)
    {
        //On accepte une connexion
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket == -1)
        {
            perror("\033[0;31mAccept failed !!!");
            reset();
            exit(EXIT_FAILURE);
        }
        else
        {
            green();
            printf("Client connected from %s to : %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            reset();
        }
        //On crée un processus pour chaque client
        client_pid = fork();
        if (client_pid == 0)
        {
            //On ferme la connexion du server
            close(server_fd);
            //On lance la fonction chat
            chat(new_socket);
            freeaddrinfo(result);// Ici on libère la mémoire allouée par getaddrinfo
            exit(0);
        }
        else
        {
            //On ferme la connexion du client
            close(new_socket);
        }
    }
   
    return 0;
    
}
