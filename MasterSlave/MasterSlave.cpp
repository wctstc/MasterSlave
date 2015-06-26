
#include "MasterSlave.h"

#include "Config.h"
#include "LogManage.h"

MasterSlave::MasterSlave(void):m_service( NULL )
{
}

MasterSlave::~MasterSlave(void)
{
}

int MasterSlave::Start()
{
	const string FileName = "MasterSlave.ini";
	const string LocalSetion = "Master";
	const string PeerSetion = "Slave";
	const string IP = "ip";
	const string Port = "port";
	const int    TimeOut = 10;


	Config config( FileName );
	string ip;
	string port;

	//Service
	config.GetKeyValue( LocalSetion, IP, ip );
	config.GetKeyValue( LocalSetion, Port, port );

	if( m_service != NULL )
		delete m_service;
	m_service = new AccessPoint;
	if( m_service->Start( this, atoi( port.c_str() ), ip ) < 0 )
	{
		CLog( "创建Master[%s:%s]失败", ip.c_str(), port.c_str() );
		return -1;
	}
	CLog( "创建Master[%s:%s]成功", ip.c_str(), port.c_str() );


	//OtherService
	string setion = PeerSetion;
	string setion_append;
	ConnectPoint *other_service;

	for( int i = 0; ;i++ )
	{
		setion_append = setion;
		setion_append.append( 1, '0' + i );

		config.GetKeyValue( setion_append.c_str(), "ip", ip );
		config.GetKeyValue( setion_append.c_str(), "port", port );

		if( ip.empty() || port.empty() )
		{
			CLog( "创建第%d个Slave[%s:%s]失败", i, ip.c_str(), port.c_str() );
			break;
		}

		other_service = new ConnectPoint( );
		if( other_service->Start( ip, atoi( port.c_str() ), TimeOut ) <= 0 )
		{
			CLog( "创建slave[%s:%s]失败", ip.c_str(), port.c_str() );
		}
		else
		{
			CLog( "创建slave[%s:%s]成功", ip.c_str(), port.c_str() );
		}
		m_other_service.push_back( other_service );

	}
	Thread::CreateThread( &m_keep_connect_thread, KeepTryConnect, this );
	
	Thread::CreateThread( &m_on_recieve, OnRecieve, this );
	
	return 1;
}

void MasterSlave::OnRecieve(string buffer)
{
	m_point_manager
}


void* MasterSlave::KeepTryConnect(void *parent)
{
	const int    TimeOut = 10;

	MasterSlave *master_slave = (MasterSlave*)parent;
	ConnectPoint *connect_point = NULL;
	int index = 0;
	int size = master_slave->m_other_service.size();

	do
	{
		if( master_slave->m_other_service[ index ]->IsConnect() == false )
		{
			connect_point = master_slave->m_other_service[ index ];
			connect_point->Start( TimeOut );
		}
		index++;
		if( index >= size )
		{
			index = 0;
			//Sleep( 10000 );
		}
	}
	while(true);
}

void* MasterSlave::OnRecieve(void *parent)
{
	MasterSlave *master_slave = ( MasterSlave * )parent;

	int size = master_slave->m_other_service.size();
	ConnectPoint *connect_point;
	string buffer;

	for( int i = 0; ;i++ )
	{
		if( i >= size )
			i = 0;

		connect_point = master_slave->m_other_service[i];

		//if( connect_point->OnRecvBuffer( buffer, 1 ) > 0 )
		//	master_slave->OnRecieve( buffer );

	}
}
