#pragma once

#include "AccessPoint.h"
#include "ConnectPoint.h"
#include "OnRecieverInterface.h"


class PointManager : public OnRecieverInterface
{
public:
	PointManager(void);
	~PointManager(void);
public:
	//virtual void OnRecv( string buffer );

public:
	//void Forword( );

	void RegistReciever( OnRecieverInterface *reciever, string ip, int port );
	void RegistReciever( OnRecieverInterface *reciever, ConnectPoint *sender );
	void UnReginstReciever( OnRecieverInterface *reciever );

private:
	//AccessPoint *m_server;
	map< OnRecieverInterface * , ConnectPoint* > m_client;

};
