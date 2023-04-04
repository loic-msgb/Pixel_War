#include <sys/ioctl.h> // pour get_terminal_size
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <b64/cencode.h>
#include <b64/cdecode.h>

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
        matrice[i] = malloc(C * sizeof(Pixel));
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
    size_t input_length = nb_lignes * nb_colonnes * sizeof(struct Pixel);
    size_t output_length = input_length * 4 / 3 + 1;
    char* buffer = (char*)malloc(output_length);
    
    // Copier les pixels de la matrice dans le buffer
    memcpy(buffer, matrice[0], input_length);
    
    // Encoder la chaîne de caractères en base64
    char* encoded = (char*)malloc(output_length);
    base64_encodestate state;
    base64_init_encodestate(&state);
    int encoded_length = base64_encode_block(buffer, input_length, encoded, &state);
    encoded_length += base64_encode_blockend(encoded + encoded_length, &state);
    encoded[encoded_length] = '\0';
    
    // Libérer la mémoire allouée pour le buffer
    free(buffer);
    
    return encoded;
}


// Convertir la chaîne de caractère en matrice
Pixel** string_to_matrice(char* buffer, int nb_lignes, int nb_colonnes) {
    // Allouer de la mémoire pour la matrice
    Pixel** matrice = (Pixel**)malloc(nb_lignes * sizeof(Pixel*));
    for (int i = 0; i < nb_lignes; i++) {
        matrice[i] = (Pixel*)malloc(nb_colonnes * sizeof(Pixel));
    }
    
    // Décoder la chaîne de caractères en base64
    size_t input_length = strlen(buffer);
    size_t output_length = input_length * 3 / 4 + 1;
    char* decoded = (char*)malloc(output_length);
    base64_decodestate state;
    base64_init_decodestate(&state);
    int decoded_length = base64_decode_block(buffer, input_length, decoded, &state);
    decoded[decoded_length] = '\0';
    
    // Copier les pixels de la chaîne de caractères décodée dans la matrice
    int index = 0;
    for (int i = 0; i < nb_lignes; i++) {
        for (int j = 0; j < nb_colonnes; j++) {
            memcpy(&matrice[i][j], &decoded[index], sizeof(Pixel));
            index += sizeof(Pixel);
        }
    }
    
    free(decoded);
    return matrice;
}


// Initialiser une structure de pixel
void init_pixel(Pixel* pixel,unsigned char r, unsigned char g, unsigned char b)
{
    pixel->r = r;
    pixel->g = g;
    pixel->r = b;
};