#ifndef CLIENT_H
#define CLIENT_H
#include <netinet/in.h>
struct Client
{
    int socket;
    struct sockaddr_in adresse;
    int compteur;
    time_t last_set;
    struct Client *suivant;
};
typedef struct Client Client;

#endif