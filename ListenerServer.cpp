#include "ListenerServer.h"
#include <sstream>
namespace Network {
ListenerServer::ListenerServer() {
    m_server =
    {
        .sin_family = AF_INET,
        .sin_port = htons( SERWER_PORT )
    };
    m_server.sin_addr.s_addr = INADDR_ANY;
    FD_ZERO(&m_master);
    FD_ZERO(&m_read_fds);

    if( inet_pton( AF_INET, SERWER_IP, & m_server.sin_addr ) <= 0 )
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

}

void ListenerServer::close() {

    shutdown( m_socket_, SHUT_RDWR );
}
void ListenerServer::listen(std::vector<Entity*>& rooms) {


    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;


    FD_ZERO(&m_read_fds);

    //add m_master m_socket to set
    FD_SET(m_socket_, &m_read_fds);
    m_client = { };

    memset( m_buffer, 0, sizeof( m_buffer ) );


    m_activity = select( m_socket_ + 1, &m_read_fds, NULL, NULL, &read_timeout);

    if ((m_activity < 0))
    {
        printf("select error");
    }

    // tmp variables
    std::string resp;
    std::stringstream str;
    std::string tmp;
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
        //printf( "|m_client ip: %s port: %d|\n", inet_ntop( AF_INET, & m_client.sin_addr, m_buffer_ip, sizeof( m_buffer_ip ) ), ntohs( m_client.sin_port ) );
        // first two checks happen before room specific code
        if(strncmp(m_buffer, "GET_SID",7) == 0) {
            m_clients.push_back(client(m_clients.size()+1, std::string(buffer_ip)));
            resp = std::to_string(m_clients.size());

            std::cout<<"m_client registered with ID:"<<m_clients.size()<<std::endl;

        } else if(strncmp(m_buffer, "CREATE_ROOM",11) == 0) {

            str<<m_buffer;

            str>>tmp;
            str>>tmp;

            bool isGood = true;
            for(auto* it: rooms) {
                if(!tmp.compare(it->getName())) {
                    isGood = false;
                    break;
                }


            }


            if(isGood) {
                rooms.push_back(new Entity(tmp));
                std::cout<<"Creating room: "<<tmp<<" "<<rooms.size()<<std::endl;

                resp = "OK " + tmp;
            } else {
                resp = "NOK " + tmp;
                std::cout<<"Could not create room: "<<tmp<<std::endl;

            }


        }
        else if(strncmp(m_buffer, "GET_ROOMS",9) == 0) {


            resp = "Rooms:\n";
            for(auto* it: rooms) {
                resp += it->getName() + "\n";
            }


        }  else if(rooms.size() == 0) {

            resp = "No rooms!";

        }  else if(strncmp(m_buffer, "JOIN",4) == 0) {
            int num;
            str<<m_buffer;
            str>>tmp;
            str>>tmp;
            str>>num;
            std::cout<<"SessionID("<<num<<") tries to join "<<tmp<<std::endl;
            Entity *e = nullptr;
            for(auto* it: rooms) {
                // std::cout<<it->getName()<<std::endl;
                if(!tmp.compare(it->getName())) {
                    e=it;
                    break;
                }
            }
            if(e != nullptr) {
                bool test = e->addPlayer(num);
                if(test) {
                    resp = "OK";
                    std::cout<<"Adding player "<< num << " to room "<< tmp<< std::endl;
                }
                else {
                    resp = "NOK";
                }
            } else {
                resp = "NOK";
            }


        } else {
            if(strncmp(m_buffer, "GET_BOARD",9) == 0) {
                str<<m_buffer;
                str>>tmp;
                str>>tmp;

                Entity *e = nullptr;
                for(auto* it: rooms) {
                    if(!tmp.compare(it->getName())) {
                        e=it;
                        break;
                    }
                }


                if(e != nullptr) {


                    int** a = e->getBoardData();
					if(e->scoreBoard()){
                    for(int ii=0; ii<8; ii++) {
                        for(int jj=0; jj<8; jj++) {
                            resp+=std::to_string((unsigned int)(a[jj][ii]+2)) + ' ';
                        }
                    }
				}else{
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


                Entity *e = nullptr;
                for(auto* it: rooms) {
                    if(!tmp.compare(it->getName())) {
                        e=it;
                        break;
                    }
                }


                if(e != nullptr) {


                    std::string tmp2(buffer_ip);
                    //std::cout<<"User with ID: "<<data[0]<<" IP: "<<tmp2<<" issued "<<tmp<<" "<<data[1]<<" "<<data[2]<<" "<<data[3]<<" "<<data[4]<<std::endl;
                    if(!tmp2.compare(m_clients[data[0]-1].name)) {
                        if (e->checkMove(data[1]-1,data[2]-1,data[3]-1,data[4]-1, data[0]) ) {
                            e->changeCurrentPlayer();
                            std::cout<<"Changing player.(ROOM: "<<tmp<<" )"<<std::endl;

                        }
                    }else{
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
        //return data(false, m_resp, strlen( m_resp ));

    }
    //return data();

}


}
