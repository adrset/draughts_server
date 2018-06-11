#include "ListenerServer.h"
#include "Entity.h"

#include <vector>
#include <string>
#include <sstream>
#include <iterator>
using namespace Network;

int main(int argc, char** argv)
{
	
	if(argc < 3)
		return 1;
	std::string ip(argv[1]);
	int port = std::atoi(argv[2]);
    ListenerServer* server = new ListenerServer(ip, port);
    //Entity* room = new Entity();
    std::vector<Entity*> rooms;

    while(1) {

        /*data d = */server->listen(rooms);

    }
    delete server;

	return 0;



}
