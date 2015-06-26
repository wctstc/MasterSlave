
#include <winsock2.h>

#include "AccessPoint.h"
#include "LogManage.h"

AccessPoint::AccessPoint(  ):m_socket_access( SOCKET_ERROR ),m_start( 0 )
{
}

AccessPoint::~AccessPoint(void)
{
	if( m_socket_access != SOCKET_ERROR )
	{
		closesocket( m_socket_access );
	}
}

int AccessPoint::Start( int port, string ip  )
{

	if( m_socket_access != SOCKET_ERROR )
	{
		closesocket( m_socket_access );
	}

	//创建socket
	m_socket_access = socket(AF_INET, SOCK_STREAM, 0); 
	if( SOCKET_ERROR == m_socket_access )
	{
		//启动服务
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		m_socket_access = socket(AF_INET, SOCK_STREAM, 0); 
		if( SOCKET_ERROR == m_socket_access )
		{
			return -1;
		}
	}

	//绑定
	struct sockaddr_in addr;
	memset(&addr, 0x00,sizeof(addr));
	addr.sin_family = AF_INET;
	if ( ip.empty() )
	{
		addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
	}

	addr.sin_port = htons( port );
	if( SOCKET_ERROR == bind( m_socket_access, (struct sockaddr *)&addr, sizeof(addr)) )
	{
		closesocket( m_socket_access );
		return -1;
	}

	//监听
	if( SOCKET_ERROR == listen( m_socket_access, 10 ))
	{
		closesocket( m_socket_access );
		return -1;
	}

	CLog( "绑定[%d]端口", port );
	
	m_start = 1;
	Thread::CreateThread( &m_access_thread, Accept, this );

	return 0;
}


int AccessPoint::Stop()
{
	m_start = false;
	closesocket( m_socket_access );
	
}

int AccessPoint::SendContent(unsigned int socket, string &content)
{
	for( int i = 0; i < m_client.size(); i++ )
	{
		if( m_client[i]->GetSocket( ) == socket )
			return m_client[i]->SendBuffer( content, 0 );
	}
	return 0;
}

int AccessPoint::SendBroadcast( string &broadcast)
{
	int num = 0;
	for( int i = 0; i < m_client.size(); i++ )
	{
		if( m_client[i]->SendBuffer( broadcast, 0 ) > 0 )
		{
			num++;
		}
	}
	return num;
}

void *AccessPoint::Accept( void *bind )
{

	struct sockaddr_in stPeerAddr;
	int nAddrLen = sizeof(sockaddr);


	AccessPoint *access_point = ( AccessPoint * ) bind;

	while( m_start )
	{
		//接收连接点
		unsigned int accept_socket = accept( access_point->m_socket_access,reinterpret_cast<sockaddr*>(&stPeerAddr),&nAddrLen);
		if ( SOCKET_ERROR == accept_socket )
			return NULL;
		
		//创建连接点
		ConnectPoint *connect_point = new ConnectPoint();
		string local_ip;
		int local_port;
		string peer_ip;
		int peer_port;

		//获取本地信息
		struct sockaddr_in local_address;
		int len = sizeof( local_address );
		getsockname( access_point->m_socket_access,(struct sockaddr *)&local_address,&len );

		local_ip   = inet_ntoa( local_address.sin_addr);
		local_port = ntohs( local_address.sin_port );

		//获取对方信息
		struct sockaddr_in peer_address;
		len = sizeof( peer_address );
		getpeername( accept_socket,(struct sockaddr *)&peer_address,&len );

		peer_ip   = inet_ntoa( peer_address.sin_addr);
		peer_port = ntohs( peer_address.sin_port );

		connect_point->Init( accept_socket, true, false, local_ip, local_port, peer_ip, peer_port, access_point->m_reciever );

		OnAccept( connect_point );

		//打印日志
		CLog( "收到来自%s:%d连接", peer_ip.c_str(), peer_port );
	}
	return NULL;
}
