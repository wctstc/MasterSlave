#include "PointManager.h"

PointManager::PointManager(void)
{
}

PointManager::~PointManager(void)
{
}

void PointManager::OnRecv(string buffer)
{

}

void PointManager::RegistReciever( OnRecieverInterface *reciever, string ip, int port)
{
	map< OnRecieverInterface * , ConnectPoint * >::iterator map_it = m_client.find( reciever );
	if( map_it != m_client.end() )
	{
		delete map_it->second;
		map_it->second = NULL;
	}


	ConnectPoint *connect_point = new ConnectPoint( reciever );
	connect_point->Start( ip, port );

	m_client[reciever] = connect_point;
}

void PointManager::RegistReciever(OnRecieverInterface *reciever, ConnectPoint *sender)
{
	m_client[reciever] = sender;
}
