#include "ProcData.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

ProcData::ProcData() : Data()
{
}

ProcData::~ProcData()
{
}

string *ProcData::getFileName(void)
{
	return &m_filename;
}

void ProcData::setFileName(string *name)
{
	m_filename = *name;
}
