#include"/home/loic/réseaux/PixelWar/serveur/Client.h"

// Fonction pour supprimer le client de la liste chaînée
void supprimer_client(Client** liste, int socket_client);

// Fonction pour afficher la liste de clients connectés
void afficher_clients_connectes(Client* liste);