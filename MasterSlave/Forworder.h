#pragma once
#include <string>
#include <vector>
#include <map>

#include "Packet.h"
#include "OnRecieverInterface.h"
#include "Sender.h"

using namespace std;

class Forworder
{
public:
	Forworder(void);
	~Forworder(void);

public:
	void Forword( string content );//网络->应用层
	void Forword( Packet packet );//应用程序->网络

	int Register( string ID, OnRecieverInterface *reciever );
	int Register( string ID, ConnectPoint *point );
	int UnRegister( string ID, OnRecieverInterface *reciever );
	int UnRegister( string ID, ConnectPoint *point );
private:
	string GetIDFromContent( string content );
	void Struct2String( string &content, Packet packet );
	void String2Struct( string &content, Packet packet );
private:
	map< string , vector<OnRecieverInterface *> > m_recievers;
	map< string , vector<ConnectPoint *> > m_points;
};
