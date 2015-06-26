#pragma once
#include <string>
#include <vector>
#include <map>

#include "Packet.h"
#include "OnRecieverInterface.h"

using namespace std;

class Forworder
{
public:
	Forworder(void);
	~Forworder(void);

public:
	void Forword( string content );
	void Forword( Packet packet );

	int Register( string ID, OnRecieverInterface *reciever );
	int UnRegister( string ID, OnRecieverInterface *reciver );
	int Register( string ID, Sender *sender );
	int UnRegister( string ID, Sender *sender );
private:
	string GetIDFromContent( string content );
	void Struct2String( string &content, Packet packet );
	void String2Struct( string &content, Packet packet );
private:
	map< string , vector<OnRecieverInterface *> > m_recievers;
	map< string , vector<Sender *> > m_senders;
};
