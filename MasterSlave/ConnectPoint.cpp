
#include <winsock2.h>

#include "ConnectPoint.h"
#include "LogManage.h"
#include "Thread.h"

int ConnectPoint::m_ID_Distribution = 0;

ConnectPoint::ConnectPoint( OnRecieverInterface *reciever ):m_local_ip( "0.0.0.0" ), m_local_port( 0 ),
																	m_peer_ip( "0.0.0.0" ), m_peer_port( 0 ),
																	m_socket( SOCKET_ERROR ),
																	m_is_connect( false ),
																	m_direction( true ),
																	m_reciever( reciever )

{
	m_connect_point_id = ++m_ID_Distribution;

	CLog( "编号[%d]连接点，构造",m_connect_point_id );
}

ConnectPoint::~ConnectPoint(void)
{
	if( m_socket != INVALID_SOCKET )
	{
		closesocket( m_socket );
	}
	CLog( "编号[%d]连接点，析构",m_connect_point_id );
}

void ConnectPoint::Init(int socket, int is_connect, int direction, string local_ip,int local_port, string peer_ip, int peer_port, OnRecieverInterface *reciever )
{
	m_socket     = socket;
	m_is_connect = is_connect;
	m_direction  = direction;
	m_local_ip   = local_ip;
	m_local_port = local_port;
	m_peer_ip    = peer_ip;
	m_peer_port  = peer_port;	
	m_reciever   = reciever;

	Thread::KillThread( &m_pthread_id );
	Thread::CreateThread( &m_pthread_id, OnRecvBuffer, this );

	CLog( "编号[%d]连接点,初始化:本端地址[%s,%d],对端地址[%s,%d],套接字[%d],连接方向[%d],连接状态[%d], 接收者个数[%d]", 
			m_connect_point_id,               //编号
			m_local_ip.c_str(), m_local_port, //本端地址
			m_peer_ip.c_str(),  m_peer_port,  //对端地址
			m_socket,                         //套接字
			m_direction,                      //连接方向
			m_is_connect                      //连接状态
			m_reciever.size() );              //接收者个数
}


int ConnectPoint::Start( int reconnect, string ip, int port, int time_out )
{
	m_direction = 1;
	m_peer_ip   = ip;
	m_peer_port = port;

	return Start( reconnect, time_out );
}

int ConnectPoint::Start( int reconnect, int time_out)
{

	m_direction = 1;
	m_reconnect = reconnect;

	if( m_socket != SOCKET_ERROR )
	{
		closesocket( m_socket );
		m_socket = SOCKET_ERROR;
	}


	//创建socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if( SOCKET_ERROR == m_socket )
	{	
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		m_socket = socket(AF_INET, SOCK_STREAM, 0); 
		if( SOCKET_ERROR == m_socket )
		{
			CLog( "%s","Error at Connect socket()" );
			return -1;
		}
	}




	//设置socket为非阻塞
	unsigned long mode = 1;
	int result = ioctlsocket( m_socket, FIONBIO, &mode);


	//创建连接
	sockaddr_in addrServer; 
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = inet_addr( m_peer_ip.c_str());
	addrServer.sin_port = htons( m_peer_port );

	connect( m_socket, (struct sockaddr*) &addrServer, sizeof(addrServer) );

	//异步等待连接成功
	fd_set write, err;
	FD_ZERO(&write);
	FD_ZERO(&err);
	FD_SET(m_socket, &write);
	FD_SET(m_socket, &err);


	TIMEVAL timeval;
	timeval.tv_sec  = time_out;
	timeval.tv_usec = 0;

	select(0, NULL, &write, &err, &timeval);
	if( FD_ISSET(m_socket, &write) > 0 )
	{
		//获取本地信息
		struct sockaddr_in local_address;
		int len = sizeof( local_address );
		getsockname( m_socket,(struct sockaddr *)&local_address,&len );

		m_local_ip   = inet_ntoa( local_address.sin_addr);
		m_local_port = ntohs( local_address.sin_port );

		//连接状态改变
		m_is_connect = 1;
		CLog( "编号[%d]连接点,连接成功:本端地址[%s,%d],对端地址[%s,%d],套接字[%d],连接方向[%d],连接状态[%d]", 
				m_connect_point_id,               //编号
				m_local_ip.c_str(), m_local_port, //本端地址
				m_peer_ip.c_str(),  m_peer_port,  //对端地址
				m_socket,                         //套接字
				m_direction,                      //连接方向
				m_is_connect );                   //连接状态

		Thread::KillThread( &m_pthread_id );
		Thread::CreateThread( &m_pthread_id, OnRecvBuffer, this );

		return true;
	}

	m_is_connect = 0;
	CLog( "编号[%d]连接点,连接失败:本端地址[%s,%d],对端地址[%s,%d],套接字[%d],连接方向[%d],连接状态[%d]", 
		m_connect_point_id,               //编号
		m_local_ip.c_str(), m_local_port, //本端地址
		m_peer_ip.c_str(),  m_peer_port,  //对端地址
		m_socket,                         //套接字
		m_direction,                      //连接方向
		m_is_connect );                   //连接状态
	return false;

}

int ConnectPoint::SendBuffer( string &buffer, int time_out )
{		
	if( m_socket == SOCKET_ERROR || m_is_connect == false )
	{
		return 0;
	}

	//发送标志创建
	fd_set write;
	FD_ZERO(&write);
	FD_SET( m_socket, &write);

	//超时时间
	TIMEVAL timeval;
	timeval.tv_sec = time_out;
	timeval.tv_usec = 0;

	//等待是否能发送数据
	select( m_socket+1, NULL, &write, NULL, &timeval );

	if( FD_ISSET( m_socket, &write) )
	{
		int ret = send( m_socket, buffer.c_str(),buffer.length(),0);
		if( ret < 0 )
		{
			m_is_connect = false;
			return ret;
		}
		CLog( "编号[%d]连接点,发送数据：%s", buffer.c_str() );
		return ret;
	}
	return 0;
}

int ConnectPoint::BindReciever(OnRecieverInterface *reciever)
{
	if( reciever != NULL )
	{
		m_reciever = reciever;
	}
	else
	{
		return 0;
	}
	return 1;
}

std::string ConnectPoint::GetLocalIP()
{
	return m_local_ip;
}

int ConnectPoint::GetLocalPort()
{
	return m_local_port;
}

std::string ConnectPoint::GetPeerIP()
{
	return m_peer_ip;
}

int ConnectPoint::GetPeerPort()
{
	return m_peer_port;
}

int ConnectPoint::GetConnetPointID()
{
	return m_connect_point_id;
}

int ConnectPoint::GetSocket()
{
	return m_socket;
}

int ConnectPoint::IsConnect()
{
	return m_is_connect;
}


int ConnectPoint::GetReconnect()
{
	return m_reconnect;
}


int ConnectPoint::SetReconnect( int reconnect )
{
	m_reconnect = reconnect;
}

Sender * ConnectPoint::GetSender()
{
	return &m_sender;
}

int ConnectPoint::GetDirection()
{
	return m_direction;
}


int ConnectPoint::Reconnect()
{
	if( m_socket != SOCKET_ERROR )
	{
		closesocket( m_socket );
		m_socket = SOCKET_ERROR;
	}


	//创建socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if( SOCKET_ERROR == m_socket )
	{	
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		m_socket = socket(AF_INET, SOCK_STREAM, 0); 
		if( SOCKET_ERROR == m_socket )
		{
			CLog( "%s","Error at Connect socket()" );
			return -1;
		}
	}




	//设置socket为非阻塞
	unsigned long mode = 1;
	int result = ioctlsocket( m_socket, FIONBIO, &mode);


	//创建连接
	sockaddr_in addrServer; 
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = inet_addr( m_peer_ip.c_str());
	addrServer.sin_port = htons( m_peer_port );


	for( int n = 1;; n++ )
	{
		connect( m_socket, (struct sockaddr*) &addrServer, sizeof(addrServer) );

		//异步等待连接成功
		fd_set write, err;
		FD_ZERO(&write);
		FD_ZERO(&err);
		FD_SET(m_socket, &write);
		FD_SET(m_socket, &err);


		TIMEVAL timeval;
		timeval.tv_sec  = 5;
		timeval.tv_usec = 0;
		select(0, NULL, &write, &err, &timeval);

		if( FD_ISSET(m_socket, &write) > 0 )
		{
			//获取本地信息
			struct sockaddr_in local_address;
			int len = sizeof( local_address );
			getsockname( m_socket,(struct sockaddr *)&local_address,&len );

			m_local_ip   = inet_ntoa( local_address.sin_addr);
			m_local_port = ntohs( local_address.sin_port );

			//连接状态改变
			m_is_connect = 1;
			CLog( "编号[%d]连接点,连接成功:本端地址[%s,%d],对端地址[%s,%d],套接字[%d],连接方向[%d],连接状态[%d]", 
				m_connect_point_id,               //编号
				m_local_ip.c_str(), m_local_port, //本端地址
				m_peer_ip.c_str(),  m_peer_port,  //对端地址
				m_socket,                         //套接字
				m_direction,                      //连接方向
				m_is_connect );                   //连接状态

			Thread::KillThread( &m_pthread_id );
			Thread::CreateThread( &m_pthread_id, OnRecvBuffer, this );

			return 1;
		}
		CLog( "编号[%d]连接点,连接失败:本端地址[%s,%d],对端地址[%s,%d],套接字[%d],连接方向[%d],连接状态[%d],第[%d]次尝试重连", 
			m_connect_point_id,               //编号
			m_local_ip.c_str(), m_local_port, //本端地址
			m_peer_ip.c_str(),  m_peer_port,  //对端地址
			m_socket,                         //套接字
			m_direction,                      //连接方向
			m_is_connect                      //连接状态
			n );                              //第n次尝试重连

		if( m_reconnect == 0 )
			break;
	}
	return 0;
}

void * ConnectPoint::OnRecvBuffer(void *arg)
{
	ConnectPoint *connect_point = ( ConnectPoint * )arg;
	string buffer;

	do 
	{
		buffer.clear();

		if( connect_point->m_socket == SOCKET_ERROR || connect_point->m_is_connect == false )
		{
			if( m_reconnect == true )
				if( Reconnect() <= 0 )
					return NULL;
		}

		//读取标志创建
		fd_set read;
		FD_ZERO(&read);
		FD_SET( connect_point->m_socket, &read);

		//超时时间
		TIMEVAL timeval;
		timeval.tv_sec = 0;
		timeval.tv_usec = 0;

		//等待是否能发送数据
		select( connect_point->m_socket+1, &read, NULL, NULL, &timeval );

		if( FD_ISSET( connect_point->m_socket, &read) )
		{
			char temp[100];
			int length;
			do 
			{
				memset( temp, 0, 100 );
				length = recv( connect_point->m_socket, temp, 100, 0 );
				buffer.append( temp );
				if( length <=0 )
				{
					connect_point->m_is_connect = false;
					CLog( "编号[%d]连接点,断开连接:本端地址[%s,%d],对端地址[%s,%d],套接字[%d],连接方向[%d],连接状态[%d]", 
						connect_point->m_connect_point_id,               //编号
						connect_point->m_local_ip.c_str(), connect_point->m_local_port, //本端地址
						connect_point->m_peer_ip.c_str(),  connect_point->m_peer_port,  //对端地址
						connect_point->m_socket,                         //套接字
						connect_point->m_direction,                      //连接方向
						connect_point->m_is_connect );                   //连接状态

					if( m_reconnect == true )
						if( Reconnect() <= 0 )
							return NULL;
				}
				if( length < 100 )
					break;
			} 
			while (true);

			if( buffer<= 0 && buffer.empty() )
			{
				if( m_reconnect == 1)
				{
					Start()
				}
			}

			CLog( "编号[%d]连接点,接收数据：%s", buffer.c_str() );

			if( connect_point->m_reciever.empty() != true )
			{
				vector< OnRecieverInterface * >::iterator vecIt = connect_point->m_reciever.begin();

				for( ; vecIt != connect_point->m_reciever.end(); vecIt++ )
				{
					if( (*vecIt) != NULL )
					{
						(*vecIt)->OnRecv( buffer, this );
					}
				}
			}
		}
	}
	while(true);

}