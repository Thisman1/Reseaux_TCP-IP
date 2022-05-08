/*
        UDP Client
*/
#include <stdio.h>      //printf
#include <string.h>     //memset
#include <stdlib.h>     //exit(0);
#include <unistd.h>     // close();
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 1024      // Taille maximum de buffer
#define PORT 8080       // Le port sur lequel envoyer les données


//print par défaut
void reset () {
        printf("\033[0m");
}

//print en vert
void green () {
        printf("\033[0;32m");
}

/*
* Fonction die() : fonction pour afficher les erreurs dans le code.
*/
void die(char *s)
{
        perror(s);
        exit(1);
}

// Main
int main(void)
{
        struct sockaddr_in client;
        int s, slen=sizeof(client);
        char buf[BUFLEN];
        char message[BUFLEN];

        // Création de la socket UDP + remplissage de la structure 'client'

        if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
                die("\033[0;31msocket");
        }
        else
        {
                green();
                printf("Socket successfully created\n");
                reset();
        }

        memset((char *) &client, 0, sizeof(client));
        
        // Remplissage de la structure 'sockaddr_in' coté client :
        client.sin_family = AF_INET;
        client.sin_port = htons(PORT);

        if (inet_aton(SERVER , &client.sin_addr) == 0)
        {
                fprintf(stderr, "inet_aton() failed\n");
                exit(1);
        }

       

        // Conversation :

        while(1)
        {
                printf("Enter message : ");
                fgets(message, sizeof(message), stdin);

                //Envoi du message au serveur avec la fonction 'sendto()'
                if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &client, slen)==-1)
                {
                        die("\033[0;31msendto()");
                }

                // Si le message tapé est 'Exit', la connexion se ferme entre le client et le serveur.
                if(strncmp(message, "Exit", 4) == 0)
                {
                        puts("\033[0;31mClient is closing");
                        break;
                }

                //Réception de la réponse et affichage :
                //On nettoie le buffer, au cas où il a reçu des données précédemment 
                memset(buf, 0, sizeof(buf));

                //Réception des réponses avec la fonction 'recvfrom()'
                if (recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &client, &slen) == -1)
                {
                        die("\033[0;31mrecvfrom()");
                }
                if(strncmp(buf, "Exit", 4) == 0)
                {
                        puts("\033[0;31mServer is closing");
                        break;
                }
                printf("Server : ");
                puts(buf);
        }

        // Fermeture de la socket
        close(s);
        return 0;
}
