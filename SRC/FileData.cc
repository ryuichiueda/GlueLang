#include "FileData.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

FileData::FileData() : Data()
{
	m_append_mode = false;
}

FileData::~FileData()
{
}

void FileData::setAppend(void)
{
	m_append_mode = true;
}

void FileData::openFile(void)
{
	int mode = O_WRONLY | O_CREAT;
	if(m_append_mode)
		mode |= O_APPEND;

	int fd = open( m_value.c_str() , mode, 0600);
	if(fd < 3){
		m_error_msg = "file does not open.";
		throw this;
	}
	if(dup2(fd,1) < 0){
		m_error_msg = "file redirect error";
		throw this;
	}
	if( close(fd) < 0){
		m_error_msg = "file redirect error";
		throw this;
	}
}

string *FileData::getFileName(void)
{
	return &m_value;
}
