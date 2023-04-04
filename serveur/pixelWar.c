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
#include <time.h>


#include "serveur.h"
#include "../src/matrice.h"

#define DEFAULT_PORT 5000
#define LG_MESSAGE   256
#define MAX_CLIENTS 5
#define DEFAULT_RATE_LIMIT 10
#define VERSION 1.0

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
    float version = VERSION;
    int wait_time = 0;

    Client* liste = NULL;

    int L, C; // dimensions matrice Ligne, Colonne
    L = DEFAULT_L;
    C = DEFAULT_C;
    
    int rate_limit = DEFAULT_RATE_LIMIT; // taux de demande par seconde

    // Récupérer les paramètres du serveur
    int option;
    // Récupère le nombre passé en parmètre
    while ((option = getopt(argc, argv, "p:s:l:")) != -1) {
        errno = 0;
        long int arg = strtol(optarg, NULL, 10); // converti en int le nombre passé en paramètre

        switch (option) {
            case 'p':
                if (errno == 0 && arg > 0) {
                    port = (int) arg;
                } else {
                    printf("Paramètre invalide, port par défaut utilisé : %d\n", port);
                }
                break;
            case 's':
                if (errno == 0 && sscanf(optarg, "%dx%d", &L, &C) == 2)
                {
                    // conversion réussie
                } else {
                    printf("Paramètre invalide, dimensions par défaut utilisées : %dx%d\n", DEFAULT_L, DEFAULT_C);
                    L = DEFAULT_L;
                    C = DEFAULT_C;
                }
                break;
            case 'l':
                if (errno == 0 && arg > 0) {
                    rate_limit = (int) arg;
                } else {
                    printf("Paramètre invalide, taux de limite par défaut utilisé : %d\n", rate_limit);
                }
                break;
            default:
                printf("format: %s [-p port] [-s LxH] [-l RATE_LIMIT]\n", argv[0]);
                exit(1);
        }
    }


    // afficher le port et les dimensions de la matrice utilisés
    printf("Connecté sur le port %d\n", port);
    printf("Dimensions de la matrice : %dx%d\n", L, C);

    // Création de la matrice
    Pixel** matrice = init_matrice(L,C);
    char* matrice_string = matrice_to_string(matrice, L, C);

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

    //afficher_matrice(matrice, L, C);
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
            nouveau_client->compteur = 0;
            nouveau_client->last_set = time(NULL);
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
                        // envoyer les dimensions de la matrice au client
                        send_size(courant->socket, L, C);
                        set_pixel_serv(courant, matrice, L, C, rate_limit);
                        break;
                    case 2:
                        printf("veux avoir les dimensions de la matrice\n");
                        // afficher L et C sur le terminal client
                        send_size(courant->socket, L, C);
                       // afficher_matrice(matrice, L, C);
                        break;
                    case 3:
                        //matrice_string = matrice_to_string(matrice, L, C);
                        printf("veux avoir la matrice\n");
                        // envoyer les dimensions de la matrice au client
                        send_size(courant->socket, L, C);
                        // envoyer la matrice au client
                        send(courant->socket, matrice_string, sizeof(struct Pixel)*L*C, 0);
                        break;
                    case 4:
                        // veut se déconnecter
                        // supprimer le client de la liste
                        printf("Déconnexion du client (socket %d)\n", courant->socket);
                        supprimer_client(&liste, courant->socket);
                        break;
                    case 5:
                        // veut avoir le temps d'attente restant
                        printf("veux avoir le temps d'attente restant\n");
                        // envoyer le temps d'attente restant au client
                        wait_time = get_wait_time(rate_limit, courant);
                        send(courant->socket, &wait_time, sizeof(int), 0);
                        break;
                    case 6:
                        // veut avoir la version du protocole
                        printf("veux avoir la version du protocole\n");
                        // envoyer la variable version au client
                        send(courant->socket, &version, sizeof(float), 0);
                        break;
                    default:
                        printf("choix invalide\n");
                        // Envoi d'un message d'erreur au client
                        send(courant->socket, "99 Unknown Command", 19, 0);
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