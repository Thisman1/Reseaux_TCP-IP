
#include <sys/socket.h>
#include <netinet/in.h>
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
#define PORT 8080

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
        
        //Read the message sent by the server
        read_size = recv(sockfd, msg1, BUFFER_SIZE, 0);
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
        //Si le message est "Exit" le server va fermer la connexion
        if(strncmp(msg2, "Exit", 4) == 0)
        {
            puts("Server is closing");
            break;
        }
        fgets(msg2, BUFFER_SIZE, stdin);
        //On envoie le message au client
        write(sockfd, msg2, strlen(msg2)+1);


    }
}

//Main function
int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    //Création du socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket successfully created\n");
    }
    //Paramétrage du socket
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    //Initialisation de l'adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    //Binding socket to port
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //Listening
    if(listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //On attend une connexion
    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    //On lance la fonction chat
    chat(new_socket);
    //On ferme le socket
    close(new_socket);
    return 0;
}
