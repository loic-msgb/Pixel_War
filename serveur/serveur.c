#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>

#include "serveur.h"

#define LG_MESSAGE 256

// Fonction pour supprimer le client de la liste chaînée
void supprimer_client(Client** liste, int socket_client) {
    Client* courant = *liste;
    Client* precedent = NULL;

    // Parcourir la liste pour trouver le client à supprimer
    while (courant != NULL && courant->socket != socket_client) {
        precedent = courant;
        courant = courant->suivant;
    }

    // Si on a trouvé le client à supprimer
    if (courant != NULL) {
        // Si le client à supprimer est le premier de la liste
        if (precedent == NULL) {
            *liste = courant->suivant;
        } else {
            precedent->suivant = courant->suivant;
        }
        // Fermer la socket du client et libérer la mémoire allouée
        close(courant->socket);
        free(courant);
    }
}


// Fonction pour afficher les adresses IP et les ports des clients connectés
void afficher_clients_connectes(Client* liste)
{
    Client* courant = liste;
    while (courant != NULL) {
        struct sockaddr_in adresse_client;
        socklen_t longueurAdresse = sizeof(struct sockaddr_in);
        getpeername(courant->socket, (struct sockaddr *)&adresse_client, &longueurAdresse);
        printf("Client %s:%d (socket %d) connecté.\n", inet_ntoa(adresse_client.sin_addr), ntohs(adresse_client.sin_port), courant->socket);
        courant = courant->suivant;
    }
}

// Fonction pour modifier le pixel demandé par le client
void set_pixel_serv(int socket_client, Pixel** matrice, int L, int C)
{
    // Recevoir le message du client
    char* message = (char*)malloc(sizeof(char)*LG_MESSAGE);
    memset(message, 0x00, LG_MESSAGE);
    recv(socket_client, message, LG_MESSAGE, 0);
    // gestion d'erreur
    if (message == NULL) {
        perror("Erreur lors de la réception du message");
        exit(1);
    }

    // Declarer les variables
    unsigned char x, y;
    Pixel pixel;

    // Recuperer les valeurs de x, y, r, g, b du message
    sscanf(message, "%hhu %hhu %hhu %hhu %hhu", &x, &y, &pixel.r, &pixel.g, &pixel.b);
    printf("x = %hhu, y = %hhu, r = %hhu, g = %hhu, b = %hhu\n", x, y, pixel.r, pixel.g, pixel.b);
    
    // Modifier la matrice
    matrice[x][y] = pixel;
    // envoyer un message de confirmation au client
    char* message2 = (char*)malloc(sizeof(char)*LG_MESSAGE);
    sprintf(message2, "Pixel (%hhu, %hhu) modifié avec succès", x, y);
    send(socket_client, message2, strlen(message2), 0);
    
    // Libérer la mémoire allouée
    free(message);
    free(message2);
}

// Envoyer les dimensions de la matrice au client
void send_size(int socket_client, int L, int C)
{   
    unsigned char x, y;
    x = (unsigned char)L;
    y = (unsigned char) C;
    // envoyer la taille de la matrice au client
    char* message = (char*)malloc(sizeof(char)*LG_MESSAGE);
    sprintf(message, "%hhu %hhu", x, y);
    send(socket_client, message, strlen(message), 0);
    free(message);
}