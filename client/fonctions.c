#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>

#include "../src/matrice.h"
#include "fonctions.h"

#define LG_MESSAGE   256

// Récupérer, valider et envoyer les infos pour placer un pixel
void set_pixel_cli(int sockfd, int L, int C)
{   
    int x_num, y_num;
    unsigned char x, y, r, g, b;
    char buffer[256];
    int num_read;

    // Récupérer les coordonnées du pixel
    do {
        printf("Entrez la coordonnée x du pixel (entre 0 et %d inclus) : ", L-1);
        num_read = scanf("%hhu", &x);
        if (num_read != 1 || x >= L) {
            printf("Erreur: la coordonnée x doit être un entier entre 0 et %d inclus.\n", L-1);
            while (getchar() != '\n'); // vider le tampon d'entrée
        }
    } while (num_read != 1 || x >= L);

    do {
        printf("Entrez la coordonnée y du pixel (entre 0 et %d inclus) : ", C-1);
        num_read = scanf("%hhu", &y);
        if (num_read != 1 || y >= C) {
            printf("Erreur: la coordonnée y doit être un entier entre 0 et %d inclus.\n", C-1);
            while (getchar() != '\n'); // vider le tampon d'entrée
        }
    } while (num_read != 1 || y >= C);

    // Récupérer la couleur du pixel
    do {
        printf("Entrez la couleur du pixel r (entre 0 et 255 inclus) : ");
        num_read = scanf("%hhu", &r);
        if (num_read != 1 || r > 255) {
            printf("Erreur: la valeur de la couleur r doit être un entier entre 0 et 255 inclus.\n");
            while (getchar() != '\n'); // vider le tampon d'entrée
        }
    } while (num_read != 1 || r > 255);

    do {
        printf("Entrez la couleur du pixel g (entre 0 et 255 inclus) : ");
        num_read = scanf("%hhu", &g);
        if (num_read != 1 || g > 255) {
            printf("Erreur: la valeur de la couleur g doit être un entier entre 0 et 255 inclus.\n");
            while (getchar() != '\n'); // vider le tampon d'entrée
        }
    } while (num_read != 1 || g > 255);

    do {
        printf("Entrez la couleur du pixel b (entre 0 et 255 inclus) : ");
        num_read = scanf("%hhu", &b);
        if (num_read != 1 || b > 255) {
            printf("Erreur: la valeur de la couleur b doit être un entier entre 0 et 255 inclus.\n");
            while (getchar() != '\n'); // vider le tampon d'entrée
        }
    } while (num_read != 1 || b > 255);

    // Envoyer les infos du pixel au serveur
    sprintf(buffer, "%hhu %hhu %hhu %hhu %hhu", x, y, r, g, b);
    send(sockfd, buffer, strlen(buffer), 0);
}

// Recevoir les dimensions de la matrice
void get_size(int sockfd, int *L, int *C)
{
    char buffer[LG_MESSAGE];
    unsigned char x, y;
    ssize_t lus = recv(sockfd, buffer, LG_MESSAGE, 0);
    sscanf(buffer, "%hhu %hhu", &x, &y);
    *L = (int)x;
    *C = (int)y;
}