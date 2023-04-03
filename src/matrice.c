#include <sys/ioctl.h> // pour get_terminal_size
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "matrice.h"

// Fonction pour obtenir la taille du terminal
void get_terminal_size(int* L, int* C)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *L = w.ws_row;
    *C = w.ws_col;
}
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


// Afficher la matrice centrée dans le terminal
void afficher_matrice(Pixel** matrice, int L, int C)
{
    int term_L, term_C;
    get_terminal_size(&term_L, &term_C);

    int start_x = (term_L - L) / 2;
    int start_y = (term_C - C) / 2;

    for (int x = 0; x < start_x; x++) {
        printf("\n");
    }
    for (int x = 0; x < L; x++) {
        for (int y = 0; y < start_y; y++) {
            printf(" ");
        }
        for (int y = 0; y < C; y++) {
            printf("\x1B[38;2;%d;%d;%dm█\x1B[0m", matrice[x][y].r, matrice[x][y].g, matrice[x][y].b);
        }
        printf("\n");
    }
}

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

// Initialiser une structure de pixel
void init_pixel(Pixel* pixel,unsigned char r, unsigned char g, unsigned char b)
{
    pixel->r = r;
    pixel->g = g;
    pixel->r = b;
};