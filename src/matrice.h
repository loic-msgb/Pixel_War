#ifndef MATRICE_H
#define MATRICE_H

#include "Pixel.h"
#include "../serveur/Client.h"

// Fonction pour obtenir la taille du terminal
void get_terminal_size(int* L, int* C);
// Initialiser une matrice
Pixel** init_matrice(int L, int C);

// Afficher la matrice 
void afficher_matrice(Pixel** matrice, int L, int C);

// Convertir la matrice de pixel en chaîne de caractère
char* matrice_to_string(struct Pixel** matrice, int nb_lignes, int nb_colonnes);

// Convertir la chaîne de caractère en matrice
struct Pixel** string_to_matrice(char* buffer, int nb_lignes, int nb_colonnes);

// Initialiser une structure de pixel
void init_pixel(Pixel* pixel,unsigned char r, unsigned char g, unsigned char b);

#endif