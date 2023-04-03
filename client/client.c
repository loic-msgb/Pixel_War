#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#include "../src/menu.h"
#include "../src/matrice.h"
#include "fonctions.h"

#define LG_MESSAGE 256

// Valeurs matrice
#define DEFAULT_L 60
#define DEFAULT_C 40

int main(int argc, char const *argv[])
{
    int L = DEFAULT_L;
    int C = DEFAULT_C;
    char choix[256];


    // Vérifiez que l'utilisateur a fourni l'adserveurse IP et le numéro de port du serveur
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ip_serveur> <port_serveur>\n", argv[0]);
        exit(1);
    }

    // Récupérez l'adserveurse IP et le numéro de port du serveur à partir des arguments de la ligne de commande
    char* serveur_ip = argv[1];
    char* serveur_port = argv[2];
    
    // Utilisez getaddrinfo() pour résoudre l'adserveurse IP du serveur en une structure d'adserveurse utilisable par les fonctions de socket
    struct addrinfo hintss, *serveur;
    memset(&hintss, 0, sizeof(hintss));
    hintss.ai_family = AF_INET;
    hintss.ai_socktype = SOCK_STREAM;
    getaddrinfo(serveur_ip, serveur_port, &hintss, &serveur);

    // Créer un socket TCP
    int socket_fd = socket(serveur->ai_family, serveur->ai_socktype, serveur->ai_protocol);
    if (socket_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Se connecter au serveur
    if (connect(socket_fd, serveur->ai_addr, serveur->ai_addrlen) == -1) {
        perror("connect");
        exit(1);
    }

    Pixel** matrice = init_matrice(L,C);
    
    while (atoi(choix) != 4)
    {   
        menu();

        //choix de l'utilisateur
        scanf("%s", choix);
        printf("Vous avez choisi %s\n", choix);
        switch (atoi(choix))
        {
        case 1:
            //envoyer le choix de l'utilisateur
            send(socket_fd, &choix, sizeof(choix), 0);
            printf("Vous avez choisi de placer un pixel\n");
            set_pixel_cli(socket_fd);
            // Recevoir message de confirmation
            char confirmation[LG_MESSAGE];
            recv(socket_fd, confirmation, LG_MESSAGE, 0);
            printf("%s", confirmation);
            break;
        case 2:
            // Envoyer la commande au serveur
            send(socket_fd, &choix, sizeof(choix), 0);
            // Recevoir les dimensions de la matrice
            get_size(socket_fd, &L, &C);
            // Afficher les dimensions de la matrice
            printf("La matrice fait %d lignes et %d colonnes", L, C);
            break;
        case 3:
            // Afficher la matrice
            // Envoyer la commande au serveur
            send(socket_fd, &choix, sizeof(choix), 0);
            // Recevoir la matrice_string du serveur
            char* matrice_string = malloc(L * C * sizeof(Pixel));
            recv(socket_fd, matrice_string, L * C * sizeof(Pixel), 0);

            // Convertir la matrice_string en matrice
            Pixel** matrice = init_matrice(L,C);
            matrice = string_to_matrice(matrice_string, L, C);
            
            afficher_matrice(matrice, L, C);
            break;
        case 4:
            // Quitter le programme
            // Envoyer la commande au serveur
            send(socket_fd, &choix, sizeof(choix), 0);
            printf("Vous avez choisi de quitter le programme\n");
            break;
        default:
            printf("99 Unknown Command");
            break;
        }
        
    }

    // Libérer la mémoire allouée dynamiquement pour la matrice
    for (int i = 0; i < L; i++) {
        free(matrice[i]);
    }
    free(matrice);

    // Fermer la socket
    close(socket_fd);

    // libérer les ressources de l'adresse
    freeaddrinfo(serveur);

    return 0;
}