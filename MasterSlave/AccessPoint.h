#pragma once

#include <string>
#include <vector>
#include "ConnectPoint.h"
#include "Thread.h"
#include "OnRecieverInterface.h"

using namespace std;

class AccessPoint
{
public:
	AccessPoint(void);
	~AccessPoint(void);
public:
	int Start( OnRecieverInterface *reciever, int port, string ip = ""  );
	virtual int SendContent( unsigned int socket, string &content );
	virtual int SendBroadcast( string &broadcast );
private:
	static void *OnAccept( void *bind );
private:
	string m_ip;
	int m_port;
	unsigned int m_socket_access;


	OnRecieverInterface *m_reciever;

private:
	pthread_t m_access_thread;
	vector< ConnectPoint * > m_client;
};
