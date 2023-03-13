#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    // Vérifiez que l'utilisateur a fourni l'adresse IP et le numéro de port du serveur
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ip_serveur> <port_serveur>\n", argv[0]);
        exit(1);
    }

    // Récupérez l'adresse IP et le numéro de port du serveur à partir des arguments de la ligne de commande
    char *server_ip = argv[1];
    char *server_port = argv[2];

    // Utilisez getaddrinfo() pour résoudre l'adresse IP du serveur en une structure d'adresse utilisable par les fonctions de socket
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(server_ip, server_port, &hints, &res);

    // Créez une socket TCP
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Connectez-vous au serveur
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        exit(1);
    }

    // Envoyer "bonjour" au serveur
    char message[] = "bonjour";
    int len = strlen(message);
    int bytes_sent = send(sockfd, message, len, 0);
    if (bytes_sent == -1) {
        perror("send");
        exit(1);
    }

    // Envoyez et recevez des données avec le serveur
    char buffer[BUFFER_SIZE];
    int bytes_received;
    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            perror("send");
            exit(1);
        }
        bytes_received = recv(sockfd, buffer, BUFFER_SIZE-1, 0);
        if (bytes_received == -1) {
            perror("recv");
            exit(1);
        }
        buffer[bytes_received] = '\0';
        printf("Received: %s\n", buffer);
    }

    // Fermez la socket et libérez les ressources
    close(sockfd);
    freeaddrinfo(res);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "/home/loic/réseaux/PixelWar/src/matrice.c"

#define TAILLE_MESSAGE 256
// Valeurs matrice
#define DEFAULT_L 60
#define DEFAULT_C 40

int main(int argc, char const *argv[])
{
    int L = DEFAULT_L;
    int C = DEFAULT_C;

    Pixel** matrice = init_matrice(L,C);

    // Vérifiez que l'utilisateur a fourni l'adresse IP et le numéro de port du serveur
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ip_serveur> <port_serveur>\n", argv[0]);
        exit(1);
    }

    // Récupérez l'adresse IP et le numéro de port du serveur à partir des arguments de la ligne de commande
    char* serveur_ip = argv[1];
    char* serveur_port = argv[2];
    
    // Utilisez getaddrinfo() pour résoudre l'adresse IP du serveur en une structure d'adresse utilisable par les fonctions de socket
    struct addrinfo hints, *serveur;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(serveur_ip, serveur_port, &hints, &serveur);

    // Créer un socket TCP
    int socketfd = socket(serveur->ai_family, serveur->ai_socktype, serveur->ai_protocol);
    if (socketfd == -1) {
        perror("socket");
        exit(1);
    }

    // Se connecter au serveur
    if (connect(socketfd, serveur->ai_addr, serveur->ai_addrlen) == -1) {
        perror("connect");
        exit(1);
    }

    // Recevoir les dimensions de la matrice
    int nb_lignes, nb_colonnes;
    char buffer[TAILLE_MESSAGE];
    if (recv(socketfd, buffer, TAILLE_MESSAGE, 0) == -1) {
        perror("recv");
        exit(1);
    }
    sscanf(buffer, "%d %d", &nb_lignes, &nb_colonnes);

    // Allouer dynamiquement la matrice pour stocker les pixels reçus
    matrice = allouer_matrice(nb_lignes, nb_colonnes);

    // Recevoir la matrice en tant que chaîne de caractères
    int taille_matrice = nb_lignes * nb_colonnes * sizeof(struct Pixel);
    char* matrice_str = (char*) malloc(taille_matrice);
    if (recv(socketfd, matrice_str, taille_matrice, 0) == -1) {
        perror("recv");
        exit(1);
    }

    // Convertir la chaîne de caractères en matrice de pixels
    string_to_matrice(matrice_str, nb_lignes, nb_colonnes, matrice);

    // Utiliser la matrice de pixels
    afficher_matrice(matrice, nb_lignes, nb_colonnes);

    // Libérer la mémoire allouée dynamiquement
    liberer_matrice(matrice, nb_lignes);

    free
