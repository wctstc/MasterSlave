#include "Config.h"

#define R_SECTION     (string("["))
#define L_SECTION     (string("]"))
#define KEY_VALUE_GAP (string("="))
#define LINE_END      (string("\n"))
#define SENTENCE_END  (string("\n"))
#define ANNOTATION    (string(";"))
#define BLACK         (string(" "))

Config::Config( void )
{

}

Config::Config( string file_name, string log_file_name )
{
	SetFile( file_name );
	if( !log_file_name.empty() )
		;//set log file;
}

Config::~Config(void)
{
}

void Config::SetFile( string file_name )
{
	m_file_name = file_name;
	m_content = FileIO( file_name ).LoadContent();
}

void Config::SetKeyValue( string setion, string key, string value )
{
	int index_begin = 0;
	int index_end = 0;

	if( 0 >= GetSetion( setion, index_begin, index_end ))//没找到setion，新增setion
	{
		m_content.insert( index_end, LINE_END );
		int index_enter = m_content.find_last_not_of( LINE_END, index_end );

		string append_string = LINE_END+R_SECTION+setion+L_SECTION + LINE_END + key + KEY_VALUE_GAP;
		m_content.insert( index_enter, append_string );
		index_begin = index_end = index_begin + append_string.length();
		
	}
	else if( 0 == GetValueByKey( key, index_begin, index_end ) )//没找到键值对，新增键值
	{
		m_content.insert( index_end, key+KEY_VALUE_GAP );
		index_begin = index_end = index_end+key.length()+KEY_VALUE_GAP.length();
	}
	else
	{
		m_content.erase( index_begin, index_end-index_begin+1 );//包含第一个字符和最后一个字符，所以要加上1
	}

	//修改值
	m_content.insert( index_begin, value );

	//保存配置
	FileIO(m_file_name).SaveContent( m_content );
}


void Config::GetKeyValue( string setion, string key, string &value, string defaultValue )
{	
	int index_begin = 0;
	int index_end = 0;

	if( GetSetion( setion, index_begin, index_end ) > 0 )
	{	
		if( GetValueByKey( key, index_begin, index_end ) > 0 )
		{
			value = m_content.substr( index_begin, index_end - index_begin + 1 );
			return ;
		}
	}
	value = defaultValue;
}

void Config::SetKeyValue( string setion, string key, string value, string fileName )
{
	//::WritePrivateProfileString( setion, key, value, fileName );
}
void Config::GetKeyValue( string setion, string key, string &value, string defaultValue, string fileName )
{
	char temp[200];
	//::GetPrivateProfileString( setion, key, defaultValue, temp, 200, fileName );
	value = temp;
}


int Config::GetBlock( string begin, string end, int &index_begin, int &index_end )
{
	int temp_index_end = index_end;

	//查找begin
	while( ( index_begin = m_content.find( begin, index_begin ) ) != -1 && index_begin < index_end )//找到开始
	{
		//判断是否在注释里
		int line_end_index = m_content.rfind( LINE_END, index_begin );
		int annotation_index = m_content.rfind( ANNOTATION, index_begin );
		if( annotation_index >=0 && annotation_index > line_end_index )//setion在注释中
		{
			index_begin = index_begin + 1;
			continue;
		}
		break;
	}
	
	//没找到begin
	if( index_begin > index_end )
	{
		index_begin = index_end = temp_index_end;
		return 0;
	}
	else if ( index_begin < 0 )
	{
		index_begin = index_end = temp_index_end;
		return -1;
	}
	
	index_begin = index_begin + begin.length();
	//查找end
	while( ( index_end = m_content.rfind( end, index_end ) ) != -1 && index_begin < index_end )//找到结束
	{
		//换行可能被注释注释掉
		if( end.compare( LINE_END ) != 0 )
		{
			//判断是否在注释里
			int line_end_index = m_content.rfind( LINE_END, index_begin );
			int annotation_index = m_content.rfind( ANNOTATION, index_begin );
			if( annotation_index >=0 && annotation_index > line_end_index )//setion在注释中
			{
				index_begin = index_begin + 1;
				continue;
			}
		}
		break;
	}

	//没找到end
	if( index_end < 0 || index_begin > index_end )
	{
		index_end = temp_index_end;
		return 2;
	}

	//找到begin和end
	return 1;
}


int Config::GetSetion( string setion, int &index_begin, int &index_end )
{
	int index_temp_begin = 0;
	int index_temp_end   = m_content.length()-1;
	int ret = GetBlock( R_SECTION + setion + L_SECTION, R_SECTION , index_temp_begin ,index_temp_end );
	index_begin = index_temp_begin;
	index_end   = index_temp_end;
	return ret;
}

int Config::GetValueByKey( string key, int &index_value_begin, int &index_value_end )
{
	int index_enter_begin = index_value_begin;
	int index_enter_end   = index_value_begin;

	int index_equal = 0;

	int index_key_begin;
	int index_key_end;

	string temp_key;
	while( true )
	{
		index_enter_end = m_content.find( LINE_END, index_enter_begin );
		if( index_enter_end < 0 )
		{
			index_enter_end = index_value_end;
		}
		if( index_enter_end > index_value_end )
		{
			break;
		}
		//index_enter_end = LINE_END.length();
		index_equal = m_content.find( KEY_VALUE_GAP, index_enter_begin );
		if( index_equal > 0 && index_equal < index_enter_end )
		{
			index_key_begin = m_content.find_first_not_of( BLACK, index_enter_begin );
			index_key_end   = m_content.find_last_not_of(  BLACK, index_equal-KEY_VALUE_GAP.length() );

			temp_key = m_content.substr( index_key_begin, index_key_end - index_key_begin + 1 );//包含第一个字符和最后一个字符，所以要加上1

			if( temp_key.compare( key ) == 0 )
			{
				index_value_begin = m_content.find_first_not_of( BLACK, index_equal+KEY_VALUE_GAP.length());
				if( index_enter_end == index_value_end )
				{
					index_value_end   = m_content.find_last_not_of( BLACK, index_enter_end );
				}
				else
				{
					index_value_end   = m_content.find_last_not_of( BLACK, index_enter_end - LINE_END.length() );
				}
				return 1;
			}
		}
		index_enter_begin = index_enter_end + LINE_END.length();
		if( index_enter_begin >= index_value_end )
		{
			break;
		}
	}
	return 0;
}