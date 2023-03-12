#include"matrice.h"

// Initialiser une matrice de dimension LxH
Pixel init_matrice(int L, int H)
{
    Pixel matrice[L][H];
    // Remplissage du tableau de pixels avec la valeur "blanc"
    for (int x = 0; x < L; x++) {
        for (int y = 0; y < H; y++) {
        matrice[x][y].r = 255;
        matrice[x][y].g = 255; 
        matrice[x][y].b = 255; 
        } 
}
}