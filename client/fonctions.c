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
void set_pixel_cli(int sockfd)
{   
    
    int x_num, y_num;
    unsigned char x, y, r, g, b;
    char buffer[256];
    // Récupérer les coordonnées du pixel
    printf("Entrez la coordonnée x du pixel : ");
    scanf("%hhu", &x);
    printf("Entrez la coordonnée y du pixel : ");
    scanf("%hhu", &y);

    // Récupérer la couleur du pixel
    printf("Entrez la couleur du pixel r: ");
    scanf("%hhu", &r);
    printf("Entrez la couleur du pixel g: ");
    scanf("%hhu", &g);
    printf("Entrez la couleur du pixel b: ");
    scanf("%hhu", &b);

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