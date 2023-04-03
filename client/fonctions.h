#include "../src/Pixel.h"

// Récupérer, valider et envoyer les infos pour placer un pixel
void set_pixel_cli(int sockfd, int L, int C);

// Recevoir les dimensions de la matrice
void get_size(int sockfd, int *L, int *C);