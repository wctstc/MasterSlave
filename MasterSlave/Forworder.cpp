#include "Forworder.h"

Forworder::Forworder(void)
{
}

Forworder::~Forworder(void)
{
}

void Forworder::Forword(string content)
{
	string ID = GetIDFromContent( content );

	vector< OnRecieverInterface * > &recivers = m_recievers.find( ID )->second;

	if( recivers.empty() == false )
	{
		vector< OnRecieverInterface * >::iterator it_vec = recivers.begin();

		for( ; it_vec != recivers.end(); it_vec++ )
		{
			Packet packet;

			String2Struct( content, packet );

			(*it_vec)->OnRecv( packet );
		}
	}
}

void Forworder::Forword(Packet packet)
{
	vector< Sender * > &senders = m_senders.find( ID )->second;
	if( senders.empty() == false )
	{
		vector< Sender * >::iterator it_vec = senders.begin();

		for( ; it_vec != senders.end(); it_vec++ )
		{
			string content;

			Struct2String( content, packet );

			packet.GetSender()->SendContent( content );
		}
	}
}
