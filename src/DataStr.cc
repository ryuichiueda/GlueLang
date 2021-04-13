// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "DataStr.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
using namespace std;

DataStr::DataStr() : Data()
{
	m_is_set = false;
}

DataStr::~DataStr()
{
}

void DataStr::createFifo(void)
{
	if(mkfifo(m_filename.c_str(),0700) != 0){
		m_error_msg = "fifo cannot be created";
		throw this;
	}
}

void DataStr::setFifoName(string *name)
{
	m_filename = *name;
}

void DataStr::openFifo(void)
{
	int fd = open( m_filename.c_str() ,O_WRONLY ,0600);
	if(dup2(fd,1) < 0){
		m_error_msg = "str redirect error";
		throw this;
	}
	if( close(fd) < 0){
		m_error_msg = "str redirect error";
		throw this;
	}
}

void DataStr::readFifo(void)
{
	ifstream ifs(m_filename.c_str());
	string tmp;
	string value;
	if(m_is_set){
		value = "\n";
	}

	bool isfirst = true;
	while(ifs && getline(ifs, tmp)){
		if(isfirst){
			value += tmp;
			isfirst = false;
		}else
			value += "\n" + tmp;
	}

	m_is_set = true;
	m_value += value;
}

string *DataStr::getFileName(void)
{
	return &m_filename;
}
