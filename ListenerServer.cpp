#include "ListenerServer.h"
#include <sstream>
namespace Network {
ListenerServer::ListenerServer(std::string ip, int port) {
    m_server =
    {
        .sin_family = AF_INET,
        .sin_port = htons( port )
    };
    m_server.sin_addr.s_addr = INADDR_ANY;

    FD_ZERO(&m_master);
    FD_ZERO(&m_read_fds);
    FD_ZERO(&m_inputR);

    m_readTimeout.tv_sec = 0;
    m_readTimeout.tv_usec = 10;

    if( inet_pton( AF_INET, ip.c_str(), & m_server.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }

    m_socket_ = socket( AF_INET, SOCK_DGRAM, 0 );

    if(( m_socket_ ) < 0 )
    {
        perror( "m_socket() ERROR" );
        exit( 2 );
    }

    m_len = sizeof( m_server );
    if( bind( m_socket_,( struct sockaddr * ) & m_server, m_len ) < 0 )
    {
        perror( "bind() ERROR" );
        exit( 3 );
    }
    
    std::cout<<"Starting server..." << std::endl << "Listening on interface " << ip << " port "<< port<<std::endl;
}

Entity* ListenerServer::findRoom(std::string room) {

    for(auto* it: m_rooms) {
        if(!room.compare(it->getName())) {
            return it;
        }
    }

    return nullptr;


}

bool ListenerServer::addRoom(std::string room) {

    // Determine if room exists
    Entity* find = findRoom(room);

    if(find != nullptr) {

        std::cout<<"[ Rooms ] Could not create room: "<<room<<" "<<m_rooms.size()<<std::endl;
        return false;
    } else {
        m_rooms.push_back(new Entity(room));
        std::cout<<"[ Rooms ] Creating room: "<<room<<" "<<m_rooms.size()<<std::endl;

        return true;


    }




}

void ListenerServer::close() {
    shutdown( m_socket_, SHUT_RDWR );
}

void ListenerServer::processClients() {

    for(client& c : m_clients) {
        std::chrono::duration<double> elapsedSeconds =  std::chrono::system_clock::now()-c.lastRequest;
        double time = elapsedSeconds.count();
        if(time > 10 && c.active) {
            c.active = false;
            std::cout<<"[ Timeout ]User with ID["<<c.id<<"] last request "<< time << "seconds ago! - Disconnecting!"<<std::endl;
        }
    }
}

int ListenerServer::processConsoleInput() {
    if (FD_ISSET(fileno(stdin), &m_inputR)) {
        std::string msg;
        std::getline(std::cin, msg);
        if(msg.compare(0, 4, "stop") == 0 || msg.compare(0, 4, "exit") == 0 ) {
            std::cout<<"Stopping server!"<<std::endl;
            return -1;
        } else if(msg.compare(0, 4, "help") == 0) {
            std::cout<<"Draughts server commands:"<<std::endl;
            std::cout<<"\tstop - stops the server"<<std::endl;
            std::cout<<"\tlist - lists clients and rooms"<<std::endl;
            std::cout<<"\tcreate <room> - create room"<<std::endl;
            std::cout<<"\tdelete <room> - delete room"<<std::endl;
        } else if(msg.compare(0, 7, "create ") == 0 && msg.size()> 7) {
			std::stringstream str;
			str<<msg;
			std::string room;
			str>>room;
			str>>room;
			addRoom(room);
            
        } else if(msg.compare(0, 4, "list") == 0) {
            std::cout<<"Users [ "<<m_clients.size()<<" ]"<<std::endl;

            for(client& c : m_clients) {
                std::string t = c.active == true ? "\tactive" : "\tinactive";
                std::cout<<"\tID ["<<c.id<<"]\t"<<c.pass<<"\t"<<c.name<< t <<std::endl;
            }
            
            std::cout<<"Rooms [ "<<m_rooms.size()<<" ]"<<std::endl;

            for(auto* it: m_rooms) {
                std::cout<<"\t"<<it->getName()<<std::endl;
            }


        }else{
			std::cout<<"Unknown command!"<<std::endl;
			std::cout<<"Type 'help' for a list of commands."<<std::endl;
		}

    }
    return 0;

}

int ListenerServer::listen() {
    int ret = 0;

    processClients();

    if(processConsoleInput() == -1) return -1;

    FD_ZERO(&m_inputR);
    FD_ZERO(&m_read_fds);

    FD_SET(fileno(stdin), &m_inputR);
    FD_SET(m_socket_, &m_read_fds);

    m_client = { };

    memset( m_buffer, 0, sizeof( m_buffer ) );
    select( fileno(stdin) + 1, &m_inputR, NULL, NULL, &m_readTimeout);

    m_activity = select( m_socket_ + 1, &m_read_fds, NULL, NULL, &m_readTimeout);

    if ((m_activity < 0))
    {
        printf("select error");
    }

    // tmp variables
    std::string resp;
    std::stringstream str;
    std::string tmp, tmp2;
    int a;
    //

    if (FD_ISSET(m_socket_, &m_read_fds)) {
        if( recvfrom( m_socket_, m_buffer, sizeof( m_buffer ), 0,( struct sockaddr * ) & m_client, & m_len ) < 0 )
        {
            perror( "recvfrom() ERROR" );
            exit( 4 );
        }
        strcpy(m_resp, m_buffer);
        char buffer_ip[ 128 ] = { };
        strcpy(buffer_ip, inet_ntop( AF_INET, & m_client.sin_addr, buffer_ip, sizeof( buffer_ip )));

        if(strncmp(m_buffer, "GET_SID",7) == 0) {
            str<<m_buffer;
            str>>tmp;
            str>>a;
            str>>tmp2;
            bool found = false;
            for(client& c : m_clients) {
                if(c.rand == a && std::string(buffer_ip) == c.ip) {
                    found = true;
                    a = c.id;
                    break;
                }
            }

            if(!found) {
                m_clients.push_back(client(m_clients.size()+1, std::string(buffer_ip), std::chrono::system_clock::now(), a, std::string(buffer_ip), tmp2));
                resp = std::to_string(m_clients.size());
                std::cout<<"[ Register ] client registered with ID [ "<<m_clients.size()<<" ] IP [ "<<std::string(buffer_ip)<<" ]"<<std::endl;
            } else {
                resp = std::to_string(a); // a was set to client id
                std::cout<<"[ Register ] retrying"<<std::endl;

            }


        } else if(strncmp(m_buffer, "CREATE_ROOM",11) == 0) {

            str<<m_buffer;

            str>>tmp;
            str>>tmp;

            resp = addRoom(tmp) ? "OK " + tmp : "NOK " + tmp;

        }
        else if(strncmp(m_buffer, "GET_ROOMS",9) == 0) {


            resp = "Rooms:\n";
            for(auto* it: m_rooms) {
                resp += it->getName() + "\n";
            }


        }  else if(m_rooms.size() == 0) {

            resp = "No rooms!";

        }  else if(strncmp(m_buffer, "JOIN",4) == 0) {
            int num;
            str<<m_buffer;
            str>>tmp;
            str>>tmp;
            str>>num;
            std::cout<<"[ Rooms ]( Room: "<< tmp << " ) User ID [ "<<num<<" ] tries to join"<<std::endl;
            Entity *e = findRoom(tmp);

            if(e != nullptr) {
                bool test = e->addPlayer(num);
                if(test) {
                    resp = "OK ";
                    resp+= std::to_string(e->getPlayers());
                    std::cout<<"[ Rooms ]( Room: "<< tmp << " ) Adding player with ID [ "<< num << " ]" << std::endl;
                }
                else {
                    resp = "NOK";
                    std::cout<<"[ Rooms ]( Room: "<< tmp << " ) player with ID [ "<< num << " ] not added." << std::endl;

                }
            } else {
                resp = "NOK";
            }


        } else {
            if(strncmp(m_buffer, "GET_BOARD",9) == 0) {


                str<<m_buffer;
                str>>tmp;
                str>>tmp;
                int id;
                str>>id;

                m_clients[id - 1].lastRequest = std::chrono::system_clock::now();

                Entity *e = findRoom(tmp);

                if(e != nullptr) {


                    int** a = e->getBoardData();
                    if(e->scoreBoard()) {
                        for(int ii=0; ii<8; ii++) {
                            for(int jj=0; jj<8; jj++) {
                                resp+=std::to_string((unsigned int)(a[jj][ii]+2)) + ' ';
                            }
                        }
                        resp +=  std::to_string((-1*e->getCurrentPlayer() + 1) / 2 + 1);
                    } else {
                        resp = "END";

                    }

                } else {
                    resp = "NOK " + tmp;

                }
            } else if(strncmp(m_buffer, "MOV ", 4) == 0) {
                int data[5];
                std::stringstream str;
                str<<m_buffer;
                std::string tmp;
                str>>tmp;
                for(int i=0; i<5; i++)
                    str>>data[i];
                str>>tmp;
                std::cout<<"Room: "<<tmp<<std::endl;
                std::string pass;
                str>>pass;

                Entity *e = findRoom(tmp);

                if(e != nullptr) {

                    std::string tmp2(buffer_ip);
                    if(!tmp2.compare(m_clients[data[0]-1].name) && m_clients[data[0]-1].active && pass == m_clients[data[0]-1].pass ) {
                        if (e->checkMove(data[1]-1,data[2]-1,data[3]-1,data[4]-1, data[0]) ) {
                            e->changeCurrentPlayer();
                            std::cout<<"Changing player.(ROOM: "<<tmp<<" )"<<std::endl;

                        }
                    } else {
                        std::cout<<"But is not allowed to play in this room."<<std::endl;
                    }
                }
            }
        }


        strncpy( m_buffer, resp.c_str(), sizeof( m_buffer ) );
        if( sendto( m_socket_, m_buffer, strlen( m_buffer ), 0,( struct sockaddr * ) & m_client, m_len ) < 0 )
        {
            perror( "sendto() ERROR" );
            exit( 5 );
        }

    }
    return ret;

}


}
