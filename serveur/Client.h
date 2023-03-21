#ifndef CLIENT_H
#define CLIENT_H

struct Client
{
    int socket;
    struct sockaddr_in adresse;
    struct Client *suivant;
};
typedef struct Client Client;

#endif