#include "ListenerServer.h"
#include <sstream> 
namespace Network{
ListenerServer::ListenerServer(){
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

void ListenerServer::close(){

 shutdown( m_socket_, SHUT_RDWR );
}

data ListenerServer::listen(Entity* e)
{
	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;
	
  
     	FD_ZERO(&m_read_fds);
  
        //add m_master m_socket to set
        FD_SET(m_socket_, &m_read_fds);
        m_client = { };
       
        memset( m_buffer, 0, sizeof( m_buffer ) );
       

	m_activity = select( m_socket_ + 1, &m_read_fds , NULL , NULL , &read_timeout);

	if ((m_activity < 0)) 
        {
            printf("select error");
        }

     	if (FD_ISSET(m_socket_, &m_read_fds)) {
		if( recvfrom( m_socket_, m_buffer, sizeof( m_buffer ), 0,( struct sockaddr * ) & m_client, & m_len ) < 0 )
		{
		    perror( "recvfrom() ERROR" );
		    exit( 4 );
		}
		//printf( "|Message from m_client|: %s \n", m_buffer );
		strcpy(m_resp, m_buffer);
	       
		char m_buffer_ip[ 128 ] = { };
		//printf( "|m_client ip: %s port: %d|\n", inet_ntop( AF_INET, & m_client.sin_addr, m_buffer_ip, sizeof( m_buffer_ip ) ), ntohs( m_client.sin_port ) );

		if(strncmp(m_buffer, "GET_BOARD",9) == 0){
			int** a = e->getBoardData();
			std::string board;
			for(int ii=0;ii<8;ii++){	
				for(int jj=0;jj<8;jj++){
			
					board+=std::to_string((unsigned int)(a[jj][ii]+2)) + ' ';
				}

			}
			//std::cout<<board<<std::endl;
			
			strncpy( m_buffer, board.c_str(), sizeof( m_buffer ) );

		}else if(strncmp(m_buffer, "GET_SID",7) == 0){

			std::string board = std::to_string(++m_numClients);
			std::cout<<"m_client registered with ID:"<<m_numClients<<std::endl;
			strncpy( m_buffer, board.c_str(), sizeof( m_buffer ) );
		}else if(strncmp(m_buffer, "MOV ", 4) == 0){
			int data[5];
			std::stringstream str;
			str<<m_buffer;
			std::string tmp;
			str>>tmp;
			for(int i=0;i<5;i++)
				str>>data[i];
			std::cout<<"User with ID: "<<data[0]<<" issued "<<tmp<<" "<<data[1]<<" "<<data[2]<<" "<<data[3]<<" "<<data[4]<<std::endl;
			if (e->checkMove(data[1]-1,data[2]-1,data[3]-1,data[4]-1) ){
				e->changeCurrentPlayer();
				std::cout<<"Changing player..."<<std::endl;

			}
		}else{
	       
			strncpy( m_buffer, "MOVE UP", sizeof( m_buffer ) );
	        }
		if( sendto( m_socket_, m_buffer, strlen( m_buffer ), 0,( struct sockaddr * ) & m_client, m_len ) < 0 )
		{
		    perror( "sendto() ERROR" );
		    exit( 5 );
		}
		return data(false, m_resp, strlen( m_resp ));
    
	}
	return data();
   
}
}