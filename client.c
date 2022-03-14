#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
#define PORT 8080

//Function to chat between client and server
void chat(int sockfd)
{
    char msg1 [BUFFER_SIZE];
    char msg2 [BUFFER_SIZE];
    int read_size;
    while(1)
    {
        //Read the message sent by the server
        read_size = recv(sockfd, msg2, BUFFER_SIZE, 0);
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
        //Si le message est "Exit" le client va fermer la connexion
        if(strncmp(msg1, "Exit", 4) == 0)
        {
            puts("Client is closing");
            break;
        }
        fgets(msg1, BUFFER_SIZE, stdin);
        //Send the message to the server
        write(sockfd, msg1, strlen(msg1)+1);

    }
}

//Main function for the client side
int main()
{
    int client_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    //Create the socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket successfully created\n");
    }
    //assign the address to the socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    //connect the socket to the server
    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("connect failed. Error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Connected to the server\n");
    }

    //Function for chat
    chat(client_fd);
    //Close the socket
    close(client_fd);
    return 0;
}