#include "PointManager.h"

PointManager::PointManager(void)
{
}

PointManager::~PointManager(void)
{
}


void PointManager::RegisterConnection(string connect_point_name, ConnectPoint *connect_point)
{
	map< string, ConnectPoint *>::iterator mapIt = m_connect_point.find( connect_point_name );
	if( mapIt == m_connect_point.end() )
	{
		m_connect_point.insert( connect_point_name, connect_point );
		return 1;
	}
	return 0;
}


ConnectPoint *PointManager::UnRegisterConnection(string connect_point_name)
{
	map< string, ConnectPoint * >::iterator mapIt = m_connect_point.find( connect_point_name );
	ConnectPoint *connect_point;
	if( mapIt != m_connect_point.end() )
	{
		connect_point = (*mapIt);
		m_connect_point.erase( mapIt );
		return connect_point;
	}
	return NULL;
}

Sender * PointManager::RegistReciever(OnRecieverInterface *reciever, string connect_point_name)
{
	map< string, ConnectPoint * >::iterator mapIt = m_connect_point.find( connect_point_name );

	if( mapIt != m_connect_point.end() )
	{
		if( 0 != mapIt->second->RegisterReciever( reciever ) )
		{
			return mapIt->second->GetSender();
		}
	}

	return NULL;
}

int PointManager::UnReginstReciever(OnRecieverInterface *reciever, string connect_point_name )
{
	map< string , ConnectPoint * >::iterator mapIt = m_connect_point.find( connect_point_name );

	if( mapIt != m_connect_point.end() )
	{
		(*mapIt)->second.UnRegisterReciever( reciever );
		return 1;
	}
	return 0;
}

int PointManager::UnReginstReciever(OnRecieverInterface *reciever)
{
	int count = 0;

	map< string, ConnectPoint * >::iterator mapIt = m_connect_point.begin();

	for( ; mapIt != m_connect_point.end(); mapIt++ )
	{
		count += (*mapIt)->second->UnRegisterReciever( reciever );
	}
	return count;
}
