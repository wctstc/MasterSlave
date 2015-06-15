#include "LogManage.h"

#include <fstream>
#include <time.h>
#include <iostream>

string LogManage::m_file_name = "Log.log";


void LogManage::SetFileName( string file_name )
{
	m_file_name = file_name;
}
	
void LogManage::LogOut( char *src_file, char *function, int line, char *format, ... )
{
	char time_buffer[50];
	string src_file_name;
	char buffer[2000];

	//将可变参数格式化到buffer
	va_list list;
	va_start( list, format );
	vsnprintf_s(buffer, sizeof(buffer) - 1,sizeof(buffer) - 1, format, list );
	va_end( list );


	//获取时间
	time_t int_timer = time(NULL);
	tm struct_timer;
	localtime_s( &struct_timer, &int_timer );
	strftime( time_buffer, sizeof( time_buffer ), "%Y-%m-%d %H:%M:%S", &struct_timer );

	//获取文件名
	src_file_name = src_file;
	src_file_name = src_file_name.substr( src_file_name.find_last_of('\\')+1 );
		
	ofstream fout( m_file_name.c_str(), ios::app );

	if( fout.is_open() )
	{
		fout << time_buffer
				<< " " << src_file_name.c_str()
				<< " " << function
				<< " " << line
				<< " " << buffer << endl;
	}
	fout.close();

	cout<< time_buffer
		<< " " << src_file_name.c_str()
		<< " " << function
		<< " " << line
		<< " " << buffer << endl;

}