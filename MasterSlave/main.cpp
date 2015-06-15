


#include "ConnectPoint.h"
#include "Config.h"
#include "AccessPoint.h"
#include "ConnectPoint.h"
#include "MasterSlave.h"



int main()
{
	//Config config( "config.ini" );
	//string ip;
	//string port;
	//config.GetKeyValue( "local","ip",   ip   );
	//config.GetKeyValue( "local","port", port );

	//AccessPoint access_point;
	//ConnectPoint connect_point;
	//string sendBuffer =  "hello world" ;
	//string recvBuffer;

	//access_point.Start( 10812 );
	//connect_point.Connect( "127.0.0.1", 10812 );
	//connect_point.SendBuffer( sendBuffer );
	//connect_point.RecvBuffer( recvBuffer );

	//cout << "RecvBuffer:" << recvBuffer.c_str() << endl;

	MasterSlave ms;
	ms.Start();

	//AccessPoint access_service;
	//ConnectPoint *access_point;
	//ConnectPoint connect_point;

	//string recv_buffer;
	//string send_buffer = "ÄãºÃ";

	//access_service.Start( 10812 );
	//connect_point.Start( "127.0.0.1", 10812, 1 );

	//access_point = access_service.GetLastConnectPoint();
	//access_point->SendBuffer( send_buffer, 1 );
	//cout << "·¢ËÍ£º"<< send_buffer.c_str() <<endl;

	//connect_point.RecvBuffer( recv_buffer, 1 );

	//cout << recv_buffer.c_str() << endl;

	char buffer[100];
	do
	{
		cin >> buffer;
		cout << buffer << endl;
	}
	while( true );

	return 1;
}