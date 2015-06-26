#pragma once

#include "AccessPoint.h"

class ServicePoint : public AccessPoint, public OnRecieverInterface
{
public:
	ServicePoint(void);
	~ServicePoint(void);
public:
	virtual void OnAccept( ConnectPoint *connect_point );
	virtual void OnRecv( string buffer, ConnectPoint *connect_point );

public:
	void Forword( string &req, string &rsp );
private:

	PointManager m_point_manager;

	typedef int (ServicePoint::&Handler)( string &req, string &rsp );
	map< string , Handler > m_handler;
};
