#include "ServicePoint.h"

ServicePoint::ServicePoint(void)
{
}

ServicePoint::~ServicePoint(void)
{
}

void ServicePoint::OnAccept(ConnectPoint *connect_point)
{
	m_point_manager.RegisterConnection( atoi( connect_point->GetSocket() ), connect_point );
}

void ServicePoint::OnRecv(string buffer, ConnectPoint *connect_point)
{
	string rsp;
	Forword( buffer, rsp );

	connect_point->SendBuffer( rsp );
}

void ServicePoint::Forword(string &req, string &rsp)
{
	map< string , Handler >::iterator mapIt = m_handler.find( req );
	
	if( mapIt != m_handler.end() )
	{
		mapIt->second( req, rsp );
	}
}
