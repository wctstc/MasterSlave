#include "FileIO.h"


FileIO::FileIO( string file_name )
{
	m_file_name = file_name;
}


FileIO::~FileIO(void)
{
}

string FileIO::LoadContent()
{
	const int buffer_size = 100;
	char buffer[buffer_size];
	string content;

	ifstream in( m_file_name.c_str() );
	
	if( in.is_open() )
	{
		while( !in.eof( ) )
		{
			in.getline( buffer,buffer_size );
			content.append( buffer );
			if( !in.eof() )
			{
				content.append( 1, '\n' );
			}
		}
	}
	return content;

}


void FileIO::SaveContent( string content )
{
	ofstream out( m_file_name.c_str() );
	if( out.is_open() )
	{
		out << content.c_str();
	}
}



