#ifndef LISTENER_SERVER_H_
#define LISTENER_SERVER_H_

#define SERWER_PORT 8888
#define SERWER_IP "192.168.1.88"
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include <chrono>
#include <ctime>
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
	std::string ip;
	int rand;
	bool active;
    unsigned int id;
    std::string pass;
    std::string name;
    std::chrono::time_point<std::chrono::system_clock> lastRequest;
    client(unsigned int id, std::string name, std::chrono::time_point<std::chrono::system_clock> time,
     int rand, std::string ip, std::string pass, bool active = true):
      ip(ip), rand(rand), active(active),id(id), pass(pass), name(name), lastRequest(time){};
} client;
class ListenerServer {
    public:
        ListenerServer(std::string ip, int port);
        int listen();
        void close();

    private:
		Entity* findRoom(std::string room);
        std::vector<Entity*> m_rooms;
		int processConsoleInput();
		void processClients();
		struct timeval m_readTimeout;
	
		bool addRoom(std::string name);
		std::string ip;
		int port;
		std::vector<client> m_clients;
        fd_set m_master;   // główna lista deskryptorów plików
        fd_set m_read_fds; // pomocnicza lista deskryptorów dla select()
        fd_set m_inputR, m_inputW, m_inputE;
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
