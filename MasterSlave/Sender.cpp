#include "Sender.h"




Sender::Sender( ConnectPoint *bind )
{
	m_connect_point = bind;
}

Sender::~Sender()
{

}

int Sender::Bind( ConnectPoint *bind )
{
	m_connect_point = bind;
}

int Sender::SendContent( string buffer )
{
	m_connect_point->SendBuffer( buffer );
}









