/*
        UDP Server
*/
#include <stdio.h>      //printf
#include <string.h>     //memset
#include <stdlib.h>     //exit(0);
#include <unistd.h>     //close()
#include <arpa/inet.h>
#include <sys/socket.h>

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

//Main
int main(void)
{
        struct sockaddr_in server, client;

        int s, slen = sizeof(client) , recv_len;
        char buf[BUFLEN];
        char buf2[BUFLEN];

        //Creation de la socket UDP
        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
                die("\033[0;31msocket");
        }
        else
        {
                green();
                printf("Socket successfully created\n");
                reset();
        }

        // On vide la structure puis on la remplit avec nos paramètres souhaités
        memset((char *) &server, 0, sizeof(server));
        
        // Remplissage de la structure 'sockaddr_in' coté serveur :
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.s_addr = htonl(INADDR_ANY);

        //On lie la socket au port avec la fonction 'bind()'
        if( bind(s , (struct sockaddr*)&server, sizeof(server) ) == -1)
        {
                die("\033[0;31mbind");
        }
        else
        {
                green();
                printf("Socket successfully binded\n");
                reset();
        }

        //On attends l'arrivée de données...
        while(1)
        {
                green();
                printf("Waiting for data...\n");
                reset();
                fflush(stdout);

                //Réception de données grâce à la fonction 'recvfrom()' côté serveur
                if((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &client, &slen)) == -1)
                {
                        die("\033[0;31mrecvfrom()");
                }

                // Affichage des détails du client et des données reçues
                printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                printf("Data: %s\n" , buf);

                // Si le message reçu est 'Exit', la connexion se ferme entre le client et le serveur.
                if(strncmp(buf, "Exit", 4) == 0)
                {
                        puts("\033[0;31mClient is closing");
                        break;
                }
                memset(buf, 0, sizeof(buf));

                //Réponse du serveur au client, envoyé grâce à la fonction 'sendto()' :
                printf("Enter message : ");
                fgets(buf2, sizeof(buf2), stdin);

                if(sendto(s, buf2, sizeof(buf2), 0, (struct sockaddr*) &client, slen) == -1)
                {
                        die("\033[0;31msendto()");
                }
                if(strncmp(buf2, "Exit", 4) == 0)
                {
                        puts("\033[0;31mServer is closing");
                        break;
                }
        }

        // Fermeture de la socket
        close(s);
        return 0;
}
