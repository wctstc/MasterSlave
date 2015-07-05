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
	void Forword( string content );//����->Ӧ�ò�
	void Forword( Packet packet );//Ӧ�ó���->����

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
