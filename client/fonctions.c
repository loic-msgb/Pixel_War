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

    // // Initialiser la structure de pixel
    // Pixel pixel ;
    // init_pixel(&pixel, r, g, b);

    // Envoyer les infos du pixel au serveur
    sprintf(buffer, "%hhu %hhu %hhu %hhu %hhu", x, y, r, g, b);
    send(sockfd, buffer, strlen(buffer), 0);
    
}