#include <stdio.h>
#include "menu.h"

void menu(void)
{
    //printf("\033[2J\033[H"); // effacer l'écran et revenir au début
    printf("\033[1;34;5;7;4;20mBienvenue dans la Pixel War\033[0m\n\n"); // message d'accueil
    printf("Que souhaitez-vous faire?\n");
    printf("1. Modifier un pixel\n");
    printf("2. Afficher les dimensions de la matrice\n");
    printf("3. Afficher la matrice\n");
    printf("4. Quitter\n");
    printf("Votre choix: ");
}