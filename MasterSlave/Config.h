#pragma once
#include <string>

#include "FileIO.h"

using namespace std;

class Config
{
public:
	Config(void);
	Config( string file_name, string log_file_name = "" );
public:
	~Config(void);

public:
	void SetFile( string file_name );
	void SetKeyValue( string setion, string key, string value );
	void GetKeyValue( string setion, string key, string &value, string default_value = "" );

public:
	static void SetKeyValue( string setion, string key, string value, string file_name );
	static void GetKeyValue( string setion, string key, string &value, string default_value, string file_name );

private:
	int GetBlock( string begin, string end, int &index_begin, int &index_end );
	int GetSetion( string setion, int &index_begin, int &index_end );
	int GetValueByKey( string key, int &index_value_begin, int &index_value_end);

public:
	string m_file_name;
	string m_content;
};
