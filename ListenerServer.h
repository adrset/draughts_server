#ifndef LISTENER_SERVER_H_
#define LISTENER_SERVER_H_

#define SERWER_PORT 8888
#define SERWER_IP "192.168.1.88"
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include <vector>
#include "Entity.h"
namespace Network {

typedef struct data {
    bool empty;
    char*  response;
    size_t length;
    data(bool e = true, char* r = nullptr, size_t l = 0) : empty(e), response(r), length(l) {};
} data;

typedef struct client {
    unsigned int id;
    std::string name;
    client(unsigned int id, std::string name): id(id), name(name){};
} client;
class ListenerServer {
    public:
        ListenerServer(std::string ip, int port);
        void listen(std::vector<Entity*>& rooms);
        void close();

    private:
		std::string ip;
		int port;
		std::vector<client> m_clients;
        fd_set m_master;   // główna lista deskryptorów plików
        fd_set m_read_fds; // pomocnicza lista deskryptorów dla select()
        struct sockaddr_in m_server;
        int m_socket_;
        int m_activity;
        char m_buffer[ 4096 ] = { };
        char m_resp[ 4096 ] = { };
        socklen_t m_len;
        struct sockaddr_in m_client;
};


}

#endif
