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
	Entity* room = new Entity();

	while(room->scoreBoard()){
			
		/*data d = */server->listen(room);

	}
	delete server;
	delete room;

   



}
