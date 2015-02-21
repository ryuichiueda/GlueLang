#include "DataFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
using namespace std;

DataFile::DataFile() : Data()
{
	m_append_mode = false;
}

DataFile::~DataFile()
{
}

void DataFile::setAppend(void)
{
	m_append_mode = true;
}

void DataFile::openFile(void)
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

string *DataFile::getFileName(void)
{
	return &m_value;
}
