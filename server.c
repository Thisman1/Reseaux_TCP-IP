
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
#define BUFFER_SIZE 1024
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
            printf("Disconnected from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
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
        //On affiche le message reçu
        printf("Client: %s\n", msg1);
        fflush(stdout);
        //Enter the message to be sent
        printf("Enter the message: ");
        fgets(msg2, BUFFER_SIZE, stdin);
        //On envoie le message au client
        write(sockfd, msg2, strlen(msg2)+1);//+1 pour le \0


    }
}

//Main function
int main(int argc , char *argv[])
{
    /*int server_fd, new_socket;  
    struct addrinfo address_ip;
    struct addrinfo *result, *server;
    int res;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    pid_t client_pid;*/
	//char buffer[1024] = {0}; //Buffer pour les messages, le {0} est pour initialiser le buffer
    
			if (argc < 2)
		{
			fprintf (stderr, "ERROR !!! not enough input\n");
			exit (0);
		}
    
    //Création du socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
    {
        perror("\033[0;31mSocket failed");
        reset();
        exit(EXIT_FAILURE);
    }
    else
    {
        green();
        printf("Socket successfully created\n");
        reset();
    }
    //Paramétrage du socket
    /*if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {

        perror("\033[0;31msetsockopt (socket option)");
        reset();
        exit(EXIT_FAILURE);
    }*/
    /* Obtain address(es) matching host/port */

           memset(&address_ip, 0, sizeof(address_ip));//On utilise memset pour initialiser le struct addrinfo à 0
           address_ip.ai_family = AF_INET;    		/* Unspecific allow IPv4 or IPv6 ==> AF_UNSPEC */
           address_ip.ai_socktype = SOCK_STREAM; 	/* Type of socket, here we have a TCP socket */
           address_ip.ai_flags = AI_PASSIVE;    	/* For wildcard IP address */
           address_ip.ai_protocol = 0;				//PPROTO_TCP;          /* Any protocol */

           res = getaddrinfo(NULL, PORT, &address_ip, &result); // getaddrinfo renvoie un pointeur vers un tableau de struct addrinfo qui contient les adresses IP et les ports associés
           if (res != 0) // Si res != 0 c'est que getaddrinfo a renvoyé une erreur
			   {
				   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
				   exit(EXIT_FAILURE);
			   }
			   
	for (server = result; server != NULL; server = server->ai_next) //On parcourt le tableau de struct addrinfo
	{
		//Create the socket

		if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		{
			continue; // Si la création du socket a échoué on passe à la prochaine adresse IP
		}
		else
		{
			green();
			printf("Socket successfully created\n"); // Sinon on affiche que le socket a été créé
			reset();
		}
		int tr=1; //tr va nous permettre de vérifier si le socket est bien bindé

		// kill "Address already in use" error message
		if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) 
		{
			perror("setsockopt");
			exit(1);
		}
		if ( bind (server_fd, server->ai_addr, server->ai_addrlen) == -1)
		{
             break; 
            close(server_fd); // Si le bind a échoué on ferme le socket et on passe à la prochaine adresse IP
		}
		else
		{
            green();
            printf("Socket successfully binded\n");// Sinon on affiche que le socket a été bindé
            reset();
		}
	}
	if (server == NULL)
			{
        perror("\033[0;31m Failed to create ant the client socket.");
        exit(EXIT_FAILURE);}
    
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

    //On attend une connexion
  while (1)
  {
    if( (new_socket = accept (server_fd, (struct sockaddr*) &address, &addrlen)) < 0 )
    //socklen_t est un type de donnée pour la taille d'un socket, addrlen est un pointeur sur la taille de l'adresse du client
    {
        perror("\033[0;31maccept failed !!!");
        reset();
        exit(EXIT_FAILURE);

    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port)); //inet_ntoa convertit l'adresse IP en chaine de caractère et ntohs convertit le port en entier
    
		if ( (client_pid = fork() == 0) ) // utilisation du fork pour créer un processus fils afin de gérer les connexions
	{
		
			char buff[1024]; //Buffer pour les messages
			if (recv(new_socket, buff, sizeof(buff),0) == -1) 
			{
				perror("\033[0;31maccept failed");
			reset();
			exit(EXIT_FAILURE);
			}
		
		//On lance la fonction chat
		chat(new_socket);
		//On ferme le socket
		freeaddrinfo(result);
		close(new_socket);
		
	}
    
}

return 0;
}
