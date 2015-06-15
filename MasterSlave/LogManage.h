#ifndef __LOG_MANGAGE_H__
#define __LOG_MANGAGE_H__

#include <string>
#include <stdarg.h>

using namespace std;

#define CLog( format, ... ) LogManage::LogOut( __FILE__, __FUNCTION__ , __LINE__, format, ##__VA_ARGS__ )
class LogManage
{
public:
	static void SetFileName( string file_name );
	static void LogOut( char *src_file, char *function, int line, char *format, ... );
private:
	static string m_file_name;
};


#endif