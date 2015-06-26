#ifndef __SENDER_H__
#define __SENDER_H__

class ConnectPoint;
class Sender
{
public:
	Sender( ConnectPoint *bind = NULL );
public:
	int Bind( ConnectPoint *bind );
	int SendContent( string buffer );

private:
	ConnectPoint *m_connect_point;
};


#endif