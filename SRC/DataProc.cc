#include "DataProc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

DataProc::DataProc() : Data()
{
}

DataProc::~DataProc()
{
}

string *DataProc::getFileName(void)
{
	return &m_filename;
}

void DataProc::setFileName(string *name)
{
	m_filename = *name;
	m_value = *name;
}
