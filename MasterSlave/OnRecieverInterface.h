#pragma once
#include <string>
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
	virtual void OnRecv( string buffer ) = 0;
};
