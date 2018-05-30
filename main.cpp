#include "ListenerServer.h"
#include "Entity.h"

#include <vector>
#include <string>
#include <sstream>
#include <iterator>
using namespace Network;

int main(int argc, char** argv)
{
    ListenerServer* server = new ListenerServer();
    //Entity* room = new Entity();
    std::vector<Entity*> rooms;

    while(1) {

        /*data d = */server->listen(rooms);

    }
    delete server;





}
