#pragma once


#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class FileIO
{
public:
	FileIO( string file_name );
	~FileIO(void);

public:
	string LoadContent();
	void SaveContent( string content );

private:
	string m_file_name;
};

