#include "Packet.h"

Packet::Packet(void):m_ID(""),m_content(""),m_sender(NULL),m_reciever(NULL)
{
}

Packet::Packet( string ID, string content, Sender *sender):m_ID(ID),m_content(content),m_sender(sender),m_reciever(NULL)
{
}

Packet::~Packet(void)
{
}

string Packet::GetContent()
{
	return m_content
}

void Packet::SetContent( string content )
{
	m_content = content;
}

string Packet::GetID()
{
	return m_ID;
}

void Packet::SetID(string ID)
{
	m_ID = ID;
}

Sender * Packet::GetSender()
{
	return m_sender;

}

OnRecieverInterface * Packet::GetReciever()
{
	return m_reciever;
}

