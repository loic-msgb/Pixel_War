#include"matrice.h"

// Initialiser une matrice de dimension LxC
Pixel** init_matrice(int L, int C)
{
    Pixel** matrice = malloc(L * sizeof(Pixel*));

    for (size_t i = 0; i < L; i++)
    {
        matrice[i] = malloc(C * sizeof(Pixel*));
    }
    
    // Remplissage du tableau de pixels avec la valeur "blanc"
    for (int x = 0; x < L; x++) {
        for (int y = 0; y < C; y++) {
        matrice[x][y].r = 255;
        matrice[x][y].g = 255; 
        matrice[x][y].b = 255; 
        } 
    }
    return matrice;
};

// Afficher la matrice 
void afficher_matrice(Pixel** matrice, int L, int C)
{
    for (int x = 0; x < L; x++) {
        for (int y = 0; y < C; y++) {
            printf("\x1B[38;2;%d;%d;%dm█\x1B[0m", matrice[x][y].r, matrice[x][y].g, matrice[x][y].b);
        }
        printf("\n");
    }
};


// Convertir la matrice de pixel en chaîne de caractère
char* matrice_to_string(struct Pixel** matrice, int nb_lignes, int nb_colonnes) {
    // Allouer de la mémoire pour la chaîne de caractères
    char* buffer = (char*)malloc(sizeof(struct Pixel) * nb_lignes * nb_colonnes);
    
    // Copier les pixels de la matrice dans la chaîne de caractères
    for (int i = 0; i < nb_lignes; i++) {
        memcpy(&buffer[i * nb_colonnes * sizeof(struct Pixel)], matrice[i], nb_colonnes * sizeof(struct Pixel));
    }
    
    return buffer;
};

// Convertir la chaîne de caractère en matrice
struct Pixel** string_to_matrice(char* buffer, int nb_lignes, int nb_colonnes) {
    // Allouer de la mémoire pour la matrice
    struct Pixel** matrice = (struct Pixel**)malloc(nb_lignes * sizeof(struct Pixel*));
    for (int i = 0; i < nb_lignes; i++) {
        matrice[i] = (struct Pixel*)malloc(nb_colonnes * sizeof(struct Pixel));
    }
    
    // Copier les pixels de la chaîne de caractères dans la matrice
    for (int i = 0; i < nb_lignes; i++) {
        memcpy(matrice[i], &buffer[i * nb_colonnes * sizeof(struct Pixel)], nb_colonnes * sizeof(struct Pixel));
    }
    
    return matrice;
};