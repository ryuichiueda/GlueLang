// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "DataJob.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

DataJob::DataJob() : Data()
{
	m_pid = 0;
}

DataJob::~DataJob()
{
}
