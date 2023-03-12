# PixelWar

Pixel War est un jeu multijoueur dans lequel les joueurs doivent "peindre" une matrice à deux dimensions en utilisant des couleurs différentes. Ce dépôt contient le code du serveur Pixel War écrit en C.


## Prérequis
Le serveur Pixel War nécessite une machine avec un système d'exploitation compatible UNIX (Linux, Mac OS, etc.).  
Le programme est écrit en langage C et doit être compilé à l'aide d'un compilateur C.
## Installation

1. Clonez le dépôt en exécutant la commande suivante dans votre terminal :
```bash
 git clone https://github.com/<username>/Pixel_War.git
```
2. Accédez au répertoire serveur :
```bash
cd Pixel_War/serveur
```
3. Compilez le serveur :
```bash
gcc -Wall PixelWar.c -o serveur
```

## Utilisation
1. Lancer le serveur et spécifier le port :
```bash
./serveur -p PORT
```
