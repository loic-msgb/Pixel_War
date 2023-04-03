#ifndef SERVEUR_H
#define SERVEUR_H

#include "Client.h"
#include "../src/matrice.h"

// Fonction pour supprimer le client de la liste chaînée
void supprimer_client(Client** liste, int socket_client);

// Fonction pour afficher la liste de clients connectés
void afficher_clients_connectes(Client* liste);

// Fonction pour modifier le pixel demandé par le client
void set_pixel_serv(int socket_client, Pixel** matrice, int L, int C);

// Envoyer les dimensions de la matrice au client
void send_size(int socket_client, int L, int C);

#endif