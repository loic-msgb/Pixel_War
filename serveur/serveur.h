#ifndef SERVEUR_H
#define SERVEUR_H

#include "Client.h"
#include "../src/matrice.h"

// Fonction pour supprimer le client de la liste chaînée
void supprimer_client(Client** liste, int socket_client);

// Fonction pour afficher la liste de clients connectés
void afficher_clients_connectes(Client* liste);

#endif