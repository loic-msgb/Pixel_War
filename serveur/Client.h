typedef struct Client Client;


struct Client
{
    int socket;
    struct sockaddr_in adresse;
    Client* suivant;
};