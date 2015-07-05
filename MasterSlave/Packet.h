#pragma once
#include < string >
using namespace std;
class Sender;
class Packet
{
public:
	Packet();
	Packet( string content, ConnectPoint *point );
	virtual ~Packet();

private:
	virtual void StringToStruct( string &content );
	virtual void StructToString( string &content );

private:
	void Encode( string &content );
	void Decode( string &content );
	
public:
	string GetID();
	void SetID( string ID );
	ConnectPoint *GetPoint();

	void ToString( string &content );
private:
	string m_ID;
	ConnectPoint *m_point;
};
