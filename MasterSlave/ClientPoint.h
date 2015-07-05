#pragma once
#include "ConnectPoint.h"

class ClientPoint :	public ConnectPoint
{
public:
	ClientPoint( string ID );
	virtual ~ClientPoint(void);

public:	
	int Start( string ip, int port, int reconnect = 1, int time_out = 0 );
	int SendBuffer( string &buffer, int time_out = 0 );

	int BindReciever( OnRecieverInterface *reciever );
public:
	int IsConnect( );

	int GetReconnect();
	int SetReconnect( int reconnect );
private:
	int Reconnect();
private:
	static void *OnRecvBuffer( void *arg );
private:
	string m_ID;

	string m_ip;
	int m_port;
	int m_reconnect;  //1:需要重连     0:不需要重连
	int m_is_connect; //1:连接，         0:未连接

	unsigned int  m_socket;
	OnRecieverInterface *m_reciever;//处理接收报文的对象

	pthread_t m_pthread_id;//等待接收报文线程
};
