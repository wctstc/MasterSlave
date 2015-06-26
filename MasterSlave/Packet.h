#pragma once
#include < string >
using namespace std;
class Sender;
class Packet
{
public:
	Packet(void);
	Packet( string ID, string content, Sender *sender );
	~Packet(void);

public:
	string GetContent();
	void SetContent( string content );

	string GetID();
	void SetID( string ID );
	Sender *GetSender();
private:
	string m_ID;
	string m_content;
	Sender *m_sender;
};
