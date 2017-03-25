// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __JOB_DATA_H_
#define __JOB_DATA_H_

#include "Data.h"

class Data;

class DataProc : public Data
{
public:
	DataProc();
	virtual ~DataProc();

	virtual string *getFileName(void);
	void setFileName(string *name);
protected:
	string m_filename;
};

#endif
