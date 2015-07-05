#include "Packet.h"

Packet::Packet(void):m_ID(""),m_point( NULL )
{
}

Packet::Packet( string content, ConnectPoint *point ):m_point(point)
{
	Decode( content );
	StringToStruct( content );
}

Packet::~Packet(void)
{
	if( m_point != NULL )
		m_point = NULL;
}



void Packet::StringToStruct(string &content)
{
	m_ID = content.substr( 0, content.find_first_not_of( " " ) );
	content = content.substr( m_ID.length() );
}

void Packet::StructToString(string &content)
{
	content.append( m_ID );
	content.append( " " );
}

void Packet::Encode(string &content)
{

}

void Packet::Decode(string &content)
{

}

std::string Packet::GetID()
{
	return m_ID;
}

void Packet::SetID(string ID)
{
	m_ID = ID;
}



ConnectPoint * Packet::GetPoint()
{
	return m_point;
}

string Packet::ToString( string &content )
{
	StructToString( content );
	Encode( content );

	return content;
}
