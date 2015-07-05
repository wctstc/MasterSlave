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
	string ID = packet.GetID();

	if( ID.empty() == false )
	{
		vector< Sender * > &senders = m_senders.find( ID )->second;
		if( senders.empty() == false )
		{
			vector< Sender * >::iterator it_vec = senders.begin();

			for( ; it_vec != senders.end(); it_vec++ )
			{
				string content;

				Struct2String( content, packet );

				(*it_vec)->SendContent( content );
			}
		}
		else
		{
			//error
		}
	}
	else
	{
		string content;
		Struct2String( content , packet );
		packet.GetSender()->SendContent( content );
	}
}

int Forworder::Register(string ID, OnRecieverInterface *reciever)
{
	vector< OnRecieverInterface * > &tmp_recievers = m_recievers.find( ID )->second;
	vector< OnRecieverInterface * >::iterator it_vec = tmp_recievers.begin();

	for( ; it_vec != tmp_recievers.end(); it_vec++ )
	{
		if( (*it_vec) == reciever )
			break;
	}
	if( it_vec == tmp_recievers.end() )
	{
		tmp_recievers.push_back( reciever );
		return 1;
	}
	return 0;
}

int Forworder::Register(string ID, Sender *sender)
{
	vector< Sender * > &tmp_senders = m_senders.find( ID )->second;
	vector< Sender * >::iterator it_vec = tmp_senders.begin();

	for( ; it_vec != tmp_senders.end(); it_vec++ )
	{
		if( (*it_vec) == sender )
			break;
	}
	if( it_vec == tmp_senders.end() )
	{
		tmp_senders.push_back( sender );
		return 1;
	}
	return 0;
}

int Forworder::UnRegister(string ID, OnRecieverInterface *reciever)
{
	vector< OnRecieverInterface * > &tmp_recievers = m_recievers.find( ID )->second;
	vector< OnRecieverInterface * >::iterator it_vec = tmp_recievers.begin();

	for( ; it_vec != tmp_recievers.end(); it_vec++ )
	{
		if( (*it_vec) == reciever )
			break;
	}
	if( it_vec != tmp_recievers.end() )
	{
		tmp_recievers.erase( it_vec );
		return 1;
	}
	return 0;
}

int Forworder::UnRegister(string ID, Sender *sender)
{	
	vector< Sender * > &tmp_senders = m_senders.find( ID )->second;
	vector< Sender * >::iterator it_vec = tmp_senders.begin();

	for( ; it_vec != tmp_senders.end(); it_vec++ )
	{
		if( (*it_vec) == sender )
			break;
	}
	if( it_vec != tmp_senders.end() )
	{
		tmp_senders.erase( it_vec );
		return 1;
	}
	return 0;
}

std::string Forworder::GetIDFromContent(string content)
{
	content.substr( 0, content.get )
}
