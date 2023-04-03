#include <stdio.h>
#include <stdlib.h> /* pour exit */
#include <unistd.h> /* pour close */
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> /* pour memset */
#include <netinet/in.h> /* pour struct sockaddr_in */
#include <arpa/inet.h> /* pour htons et inet_aton */
#include <errno.h>
#include <sys/select.h>

#include "serveur.h"
#include "../src/matrice.h"

#define DEFAULT_PORT 5000
#define LG_MESSAGE   256
#define MAX_CLIENTS 5

// Valeurs matrice Lignes Colonnes
#define DEFAULT_L 60
#define DEFAULT_C 40

int main(int argc, char const *argv[])
{
    int port = DEFAULT_PORT; //port par défaut
    int socket_ecoute; // server_socket
    socklen_t longueurAdresse;
    fd_set readfds; // contient les sockets en attente de lecture
    int max_fd; //nombre maximum de filedescriptor
    struct sockaddr_in adresse_serveur, adresse_client;
    char messageRecu[LG_MESSAGE];

    Client* liste = NULL;

    int L, C; // dimensions matrice Ligne, Colonne
    L = DEFAULT_L;
    C = DEFAULT_C;
    
    // Récupère le nombre passé en parmètre
    int option;
    while ((option = getopt(argc, argv, "p:s:")) != -1) {
        switch (option) {
            case 'p':
                errno = 0; // =! 0 si il y a une erreur de conversion à la ligne suivante
                port = strtol(optarg, NULL, 10); // converti en int le nombre passé en paramètre
                if (errno != 0 || port <= 0) {
                    printf("Paramètre invalide, port par défaut utilisé : %d\n", port);
                    port = DEFAULT_PORT;
                }
                break;
            case 's':
                if (sscanf(optarg, "%dx%d", &L, &C) != 2) {
                    printf("Paramètre invalide, dimensions par défaut utilisées : %dx%d\n", DEFAULT_L, DEFAULT_C);
                    L = DEFAULT_L;
                    C = DEFAULT_C;
                }
                break;
            default:
                printf("format: %s [-p port] [-s LxC]\n", argv[0]);
                exit(1);
        }
    }
    // afficher le port et les dimensions de la matrice utilisés
    printf("Connecté sur le port %d\n", port);
    printf("Dimensions de la matrice : %dx%d\n", L, C);

    // Création de la matrice
    Pixel** matrice = init_matrice(L,C);

    // créer un socket de communication
    socket_ecoute = socket(PF_INET, SOCK_STREAM, 0);     /* 0 indique que l'on utilisera le protocole par défaut associé à SOCK_STREAM soit TCP */

    // tester la valeur renvoyée par socket()
    if (socket_ecoute < 0)
    {
        perror("socket");
        exit(-1); //code d'erreur
    }

    printf("Socket créée avec succès ! (%d)\n", socket_ecoute);

    // préparer l'adresse d'attachement locale
    longueurAdresse = sizeof(struct sockaddr_in);

    // Initialiser à 0
    memset(&adresse_serveur, 0x00, longueurAdresse);

    // Configurer le serveur
    adresse_serveur.sin_family = PF_INET;
    adresse_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
    adresse_serveur.sin_port = htons(port);

    // Attacher le socket d'écoute à l'adresse serveur
    if ((bind(socket_ecoute,(struct sockaddr*)&adresse_serveur, longueurAdresse)) < 0)
    {
        perror("bind");
        exit(-2);
    }
    printf("Socket attachée avec succès !\n");
    
    // mettre le socket en mode écoute | MAX_CLIENTS peuvent être mis en attente
    if (listen(socket_ecoute, MAX_CLIENTS) < 0) 
    {
        perror("listen");
        exit(-3);
    }
    printf("Socket placée en écoute passive ...\n");

    while (1)
    {
        // initialiser les set de file descriptor
        FD_ZERO(&readfds);
        FD_SET(socket_ecoute, &readfds);
        max_fd = socket_ecoute; // on commence avec le socket d'écoute
        // Ajouter les sockets des clients existants au readfds
        Client* courant = liste;
        while (courant != NULL)
        {
            FD_SET(courant->socket, &readfds);
            if (courant->socket > max_fd) {
                max_fd = courant->socket; // mise à jour du maximum de file descriptor
            }
            courant = courant->suivant;
        }

        // Wait for an activity on any of the sockets
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Check if there is a new incoming connection
        if (FD_ISSET(socket_ecoute, &readfds))
        {
            // Accepter la connexion entrante
            int client_len = sizeof(adresse_client);
            int client_socket = accept(socket_ecoute, (struct sockaddr *)&adresse_client, &client_len);
            if (client_socket < 0) {
                perror("accept");
                continue;
            }
            // Ajouter le nouveau client à la liste
            Client* nouveau_client = (Client*)malloc(sizeof(Client));
            nouveau_client->socket = client_socket;
            nouveau_client->suivant = liste;
            liste = nouveau_client;
            printf("Nouvelle connexion : %s : %d (socket %d)\n", inet_ntoa(adresse_client.sin_addr), ntohs(adresse_client.sin_port), client_socket);
        }

        // Vérifier si les sockets des clients ont reçu des données
        courant = liste;
        while (courant != NULL)
        {
            if (FD_ISSET(courant->socket, &readfds))
            {
                // Lecture des données reçues
                int lus = recv(courant->socket, messageRecu, LG_MESSAGE, 0);
                // tester la valeur renvoyée par recv()               
                if (lus <= 0) {
                    // Erreur ou déconnexion du client
                    if (lus == 0) {
                        printf("Déconnexion du client (socket %d)\n", courant->socket);
                        supprimer_client(&liste, courant->socket);
                    } 
                    else {
                        perror("recv");
                        exit(-4);
                    }
                } 
                else {
                    // Affichage du message reçu
                    messageRecu[lus] = '\0'; // ajout du caractère de fin de chaîne
                    printf("Message reçu (socket %d) : %s\n", courant->socket, messageRecu);
                    
                    // Conversion du message en int
                    int choix = atoi(messageRecu);
                    
                    switch (choix)
                    {
                    case 1:
                        printf("veux placer un pixel\n");
                        set_pixel_serv(courant->socket, matrice, L, C);
                        break;
                    case 2:
                        printf("veux avoir les dimensions de la matrice\n");
                        // afficher L et C sur le terminal client
                        send_size(courant->socket, L, C);
                        break;
                    case 3:
                        char* matrice_string = matrice_to_string(matrice, L, C);
                        printf("veux avoir la matrice\n");
                        // envoyer la matrice au client
                        send(courant->socket, matrice_string, sizeof(struct Pixel)*L*C, 0);
                        break;
                    case 4:
                        // veut se déconnecter
                        // supprimer le client de la liste
                        printf("Déconnexion du client (socket %d)\n", courant->socket);
                        supprimer_client(&liste, courant->socket);
                        break;
                    default:
                        break;
                    }
                }
            }
            memset(&messageRecu, 0x00, LG_MESSAGE);
            courant = courant->suivant;
            
        }
    }
    
    // Fermer les sockets des clients restants
    Client* courant = liste;
    while (courant != NULL) {
        close(courant->socket);
        courant = courant->suivant;
    }
    
    // On ferme la ressource avant de quitter   
    close(socket_ecoute);

    // Libérer chaque ligne de la matrice
    for (int i = 0; i < L; i++) {
        free(matrice[i]);
    }
    free(matrice);

    return 0;
}