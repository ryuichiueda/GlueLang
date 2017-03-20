// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __JOB_DATA_H_
#define __JOB_DATA_H_

#include "Data.h"

class Data;

class DataJob : public Data
{
public:
	DataJob();
	virtual ~DataJob();

	virtual string *getFileName(void){return NULL;}
	int m_pid;
};

#endif
