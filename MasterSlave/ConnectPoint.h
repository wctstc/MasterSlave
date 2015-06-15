#ifndef __CONNECT_POINT_H__
#define __CONNECT_POINT_H__

#include <string>
#include "OnRecieverInterface.h"
#include "Thread.h"
using namespace std;


class ConnectPoint
{
public:
	ConnectPoint( OnRecieverInterface *reciever = NULL );
	~ConnectPoint(void);

public:
	void Init(	int socket,		int is_connect, int direction,
				string local_ip,int local_port, 
				string peer_ip, int peer_port,
				OnRecieverInterface *reciever );

	int Start( int time_out );
	int Start( string ip, int port, int time_out );
	int SendBuffer( string &buffer, int time_out );

	int RegisterReciever( OnRecieverInterface *reciever );
public:
	string GetLocalIP( );
	int GetLocalPort( );
	string GetPeerIP( );
	int GetPeerPort( );

	int GetConnetPointID( );
	int GetSocket( );
	int GetDirection( );
	int IsConnect( );

private:
	static void *OnRecvBuffer( void *arg );
private:
	string m_local_ip;
	int m_local_port;
	string m_peer_ip;
	int m_peer_port;

	unsigned int  m_socket;

	int m_is_connect;//1:连接，         0:未连接
	int m_direction; //1:本方主动连接， 0:对方主动连接
	
	int m_connect_point_id;

	//处理接收报文的对象
	OnRecieverInterface *m_reciever;

	//等待接收报文线程
	pthread_t m_pthread_id;

	static int m_ID_Distribution;
};

#endif