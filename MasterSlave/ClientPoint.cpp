#include "ClientPoint.h"

#include <Windows.h>


ClientPoint::ClientPoint( string ID ):m_ID( ID )
{
	m_ip = "";
	m_port = 0;
	m_reconnect = 0;
	m_is_connect = 0;

	m_socket = SOCKET_ERROR;
	m_reciever = NULL;
}

ClientPoint::~ClientPoint(void)
{
	if( m_reciever != NULL )
		m_reciever = NULL;
}

int ClientPoint::Start( string ip, int port, int reconnect, int time_out )
{
	//初始化
	m_ip = ip;
	m_port = port;
	m_reconnect = reconnect;

	if( SOCKET_ERROR == m_socket )
	{
		closesocket( m_socket );
	}

	Thread::KillThread( &m_pthread_id );

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
	addrServer.sin_addr.s_addr = inet_addr( m_ip.c_str());
	addrServer.sin_port = htons( m_port );
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
		//连接状态改变
		m_is_connect = 1;
		CLog( "客户端连接点：ID[%s]连接至服务器[%s:%d]成功",m_ID.c_str(), m_ip.c_str(), m_port );
	
		Thread::CreateThread( &m_pthread_id, OnRecvBuffer, this );

		return true;
	}

	if( true == m_reconnect )
	{
		Thread::CreateThread( &m_pthread_id, OnRecvBuffer, this );
	}
	CLog( "客户端连接点：ID[%s]连接至服务器[%s:%d]失败",m_ID.c_str(), m_ip.c_str(), m_port );
	return false;
}

int ClientPoint::SendBuffer(string &buffer, int time_out )
{
	if( m_socket == SOCKET_ERROR || m_is_connect == false )
	{
		return 0;
	}

	int ret = send( m_socket, buffer.c_str(),buffer.length(),0);
	if( ret < 0 )
	{
		m_is_connect = false;
		return ret;
	}

	if( ret == buffer.length() )
	{
		CLog( "客户端连接点：ID[%s],发送数据[%s:%d]", m_ID, buffer.c_str(), buffer.length() );
	}
	else
	{
		CLog( "客户端连接点：ID[%s],发送数据[%d]与原始数据[%s:%d]不一致", m_ID, ret, buffer.c_str(), buffer.length() );
	}
	return ret;
}

int ClientPoint::BindReciever(OnRecieverInterface *reciever)
{
	m_reciever = reciever;
}

int ClientPoint::IsConnect()
{
	return m_is_connect;
}

int ClientPoint::GetReconnect()
{
	return m_reconnect;
}

int ClientPoint::SetReconnect(int reconnect)
{
	m_reconnect = reconnect;
}

int ClientPoint::Reconnect()
{
	int i = 0;

	do 
	{
		i++;

		//创建socket
		m_socket = socket(AF_INET, SOCK_STREAM, 0);
		if( SOCKET_ERROR == m_socket )
		{	
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
			m_socket = socket(AF_INET, SOCK_STREAM, 0); 
			if( SOCKET_ERROR == m_socket )
			{
				CLog( "客户端连接点：ID[%s]第%d次重连失败,原因套接字创建失败,等待3秒后重试",m_ID.c_str(), i );
				Sleep( 3000 );
			}
		}




		//设置socket为非阻塞
		unsigned long mode = 1;
		ioctlsocket( m_socket, FIONBIO, &mode);


		//创建连接
		sockaddr_in addrServer; 
		addrServer.sin_family = AF_INET;
		addrServer.sin_addr.s_addr = inet_addr( m_ip.c_str());
		addrServer.sin_port = htons( m_peer_port );


		do 
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
				//连接状态改变
				m_is_connect = 1;

				CLog( "客户端连接点：ID[%s]连接至服务器[%s:%d]第%d成功",m_ID.c_str(), m_ip.c_str(), m_port, i );

				return 1;
			}

			if( FD_ISSET(m_socket, &err) > 0 )
			{
				CLog( "客户端连接点：ID[%s]连接至服务器[%s:%d]第%d失败:,套接字断开",m_ID.c_str(), m_ip.c_str(), m_port, i );
				break;
			}
			i++;
			CLog( "客户端连接点：ID[%s]连接至服务器[%s:%d]第%d失败,超时",m_ID.c_str(), m_ip.c_str(), m_port, i );
			
		} while ( SOCKET_ERROR == m_socket );
	} while ( m_reconnect  );
	return 0;
}

void * ClientPoint::OnRecvBuffer(void *arg)
{
	ConnectPoint *connect_point = ( ConnectPoint * )arg;
	string buffer;
	buffer.clear();


	do
	{
		if( false == m_is_connect )
		{
			if( true == m_reconnect )
			{
				connect_point->Reconnect();
			}
			else
			{
				return NULL;
			}
		}

		//读取标志创建
		fd_set read;
		FD_ZERO(&read);
		FD_SET( connect_point->m_socket, &read);

		//超时时间
		TIMEVAL timeval;
		timeval.tv_sec = 3;
		timeval.tv_usec = 0;

		//等待是否有数据接收
		select( connect_point->m_socket+1, &read, NULL, NULL, &timeval );

		if( FD_ISSET( connect_point->m_socket, &read) )
		{
			char temp[100];
			int length;
			buffer.clear();
			do 
			{
				memset( temp, 0, 100 );
				length = recv( connect_point->m_socket, temp, 100, 0 );
				buffer.append( temp );
				if( length <=0 )
				{
					connect_point->m_is_connect = false;
					CLog( "客户端连接点：ID[%s]连接至服务器[%s:%d]断开",
						connect_point->m_ID.c_str(), 
						connect_point->m_ip.c_str(), 
						connect_point->m_port );
					break;
				}
				if( length < 100 )
					break;
			} 
			while (true);

			if( false == connect_point->m_is_connect )
			{
				continue;
			}

			CLog( "客户端连接点：ID[%s]收到数据[%s,%d]", m_ID.c_str(), buffer.c_str, buffer.length() );

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

		}while( !( false == m_is_connect && false == m_reconnect ) );

	}
}

