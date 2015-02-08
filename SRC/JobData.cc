#include "JobData.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

JobData::JobData() : Data()
{
	m_pid = 0;
}

JobData::~JobData()
{
}
