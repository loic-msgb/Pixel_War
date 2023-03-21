#include <stdio.h>
#include "menu.h"

void menu(Pixel** matrice, int L, int C)
{
    printf("\033[1;34m"); // couleur bleue en gras
    printf("\033[5m"); // clignotement
    printf("\033[7m"); // inversion des couleurs
    printf("\033[4m"); // soulignement
    printf("\033[2J"); // effacer l'écran
    printf("\033[H"); // revenir au début de l'écran
    printf("\033[20m"); // taille de police élevée
    printf("\t\t\t\t Bienvenue dans la Pixel War\n");
    printf("\033[0m"); // réinitialisation des paramètres de formatage
    printf("\n");

    // afficher la matrice
    afficher_matrice(matrice, L,C);

    printf("Que voulez vous faire ?\n");
    printf("1. Placer un pixel\t\t2. Afficher dimensions de la matrice\n");
}