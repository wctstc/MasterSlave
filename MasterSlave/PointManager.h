#pragma once

#include "AccessPoint.h"
#include "ConnectPoint.h"
#include <map>

class PointManager : public OnRecieverInterface
{
public:
	PointManager(void);
	~PointManager(void);

public:
	int RegisterConnection( string connect_point_name, ConnectPoint * );
	ConnectPoint *UnRegisterConnection( string connect_point_name );


	Sender *RegistReciever( OnRecieverInterface *reciever, string connect_point_name );
	int UnReginstReciever( OnRecieverInterface *reciever, string connect_point_name );
	int UnReginstReciever( OnRecieverInterface *reciever );

private:
	map< string , ConnectPoint* > m_connect_point;
};
