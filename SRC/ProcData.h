#ifndef __JOB_DATA_H_
#define __JOB_DATA_H_

#include "Data.h"

class Data;

class ProcData : public Data
{
public:
	ProcData();
	virtual ~ProcData();

	virtual string *getFileName(void);
	void setFileName(string *name);
protected:
	string m_filename;
};

#endif
