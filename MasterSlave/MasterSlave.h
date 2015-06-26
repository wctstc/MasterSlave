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

	
	AccessPoint *m_service;                   //�Լ���Ϊ���������ñ�ķ���������
	vector< ConnectPoint * > m_other_service; //��ķ����������������Լ���Ϊ�ͻ�������
	pthread_t m_keep_connect_thread;
	pthread_t m_on_recieve;
};
