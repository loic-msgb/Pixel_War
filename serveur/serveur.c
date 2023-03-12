#include"/home/loic/réseaux/PixelWar/serveur/serveur.h"

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