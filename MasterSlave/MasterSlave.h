#pragma once

#include "AccessPoint.h"
#include "ConnectPoint.h"
#include "PointManager.h"
#include "OnRecieverInterface.h"

class MasterSlave : OnRecieverInterface
{
public:
	MasterSlave(void);
	~MasterSlave(void);
public:
	int Start( );
	void OnRecieve( string buffer );

private:
	static void* KeepTryConnect( void *parent );
	static void* OnRecieve( void *parent );
private:
	PointManager m_point_manager;

	
	AccessPoint *m_service;                   //自己作为服务器，让别的服务器连接
	vector< ConnectPoint * > m_other_service; //别的服务器做服务器，自己作为客户端连接
	pthread_t m_keep_connect_thread;
	pthread_t m_on_recieve;
};
