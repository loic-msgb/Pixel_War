# Pixel_War

Pixel War est un jeu multijoueur dans lequel les joueurs doivent "peindre" une matrice à deux dimensions en utilisant des couleurs différentes. Ce dépôt contient le code du serveur Pixel War écrit en C.


## Prérequis
Le serveur Pixel War nécessite une machine avec un système d'exploitation compatible UNIX (Linux, Mac OS, etc.).  
Le programme est écrit en langage C et doit être compilé à l'aide d'un compilateur C.

## Installation
0. Installez la librairie libb64 :
```bash
sudo apt-get install libb64-dev
```
1. Clonez le dépôt en exécutant la commande suivante dans votre terminal :
```bash
 git clone https://github.com/loic-msgb/Pixel_War.git
```
2. Accédez au répertoire serveur :
```bash
cd Pixel_War/serveur
```
3. Compilez le serveur :
```bash
 gcc pixelWar.c serveur.c ../src/matrice.c -o serveur -L /usr/include/b64 -l b64
```
4. Accédez au répertoire client :
```bash
cd ../client
```
5. Compilez le client : 
```bash
 gcc client.c fonctions.c ../src/menu.c ../src/matrice.c -o client -L /usr/include/b64 -l b64
```

## Utilisation
1. Lancer le serveur avec les paramètres suivants {port} {nombre de lignes}x{nombre de colonnes} et nombre de pixels/min :
```bash
./serveur -p {PORT} -s {LxC} -l {RATE_LIMIT} # port par défaut : 5000
```
2. Lancer le client dans un autre terminal :
```bash
./client {localhost} {PORT}
```
