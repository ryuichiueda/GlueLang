#ifndef __JOB_DATA_H_
#define __JOB_DATA_H_

#include "Data.h"

class Data;

class JobData : public Data
{
public:
	JobData();
	virtual ~JobData();

	virtual string *getFileName(void){return NULL;}
	int m_pid;
};

#endif
