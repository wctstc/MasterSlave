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
	int Start( int port, string ip = ""  );
	int Stop();

	int Broadcast( string buffer );
public:
	void OnAccept( ConnectPoint *connect_point ) = 0;
private:
	static void *Accept( void *bind );
private:
	unsigned int m_socket_access;
	pthread_t m_access_thread;
	int m_start;
};
