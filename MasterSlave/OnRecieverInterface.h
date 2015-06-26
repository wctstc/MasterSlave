#pragma once

#include <string>
#include "Packet.h"
using namespace std;

class OnRecieverInterface
{
public:
	OnRecieverInterface(void)
	{

	}
	~OnRecieverInterface(void)
	{

	}

public:
	virtual void OnRecv( Packet packet ) = 0;
	virtual void OnClose( ) = 0;
};
