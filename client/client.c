#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include"/home/loic/réseaux/PixelWar/src/matrice.c"

#define TAILLE_MESSAGE 7200
// Valeurs matrice
#define DEFAULT_L 60
#define DEFAULT_C 40

int main(int argc, char const *argv[])
{
    int L = DEFAULT_L;
    int C = DEFAULT_C;

    Pixel** matrice = init_matrice(L,C);

    // Vérifiez que l'utilisateur a fourni l'adserveurse IP et le numéro de port du serveur
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ip_serveur> <port_serveur>\n", argv[0]);
        exit(1);
    }

    // Récupérez l'adserveurse IP et le numéro de port du serveur à partir des arguments de la ligne de commande
    char* serveur_ip = argv[1];
    char* serveur_port = argv[2];
    
    // Utilisez getaddrinfo() pour résoudre l'adserveurse IP du serveur en une structure d'adserveurse utilisable par les fonctions de socket
    struct addrinfo hints, *serveur;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(serveur_ip, serveur_port, &hints, &serveur);

    // Créer un socket TCP
    int socket_fd = socket(serveur->ai_family, serveur->ai_socktype, serveur->ai_protocol);
    if (socket_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Se connecter au serveur
    if (connect(socket_fd, serveur->ai_addr, serveur->ai_addrlen) == -1) {
        perror("connect");
        exit(1);
    }

    // Recevoir le message du serveur
    char buffer[TAILLE_MESSAGE];
    ssize_t nb_octets = recv(socket_fd, buffer, TAILLE_MESSAGE, 0);
    if (nb_octets == -1) {
        perror("recv");
        exit(1);
    }
    //buffer[nb_octets] = '\0'; // Ajouter le caractère de fin de chaîne
    
    // Convertir la chaîne de caractères en une matrice de pixels
    matrice = string_to_matrice(buffer, L, C);

    afficher_matrice(matrice, L,C);

    // Libérer la mémoire allouée dynamiquement pour la matrice
    for (int i = 0; i < L; i++) {
        free(matrice[i]);
    }
    free(matrice);

    // Fermer la socket
    close(socket_fd);

    return 0;
}