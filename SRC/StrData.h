#ifndef __STR_DATA_H_
#define __STR_DATA_H_

#include "Data.h"

class Data;
class Condition;

class StrData : public Data
{
public:
	StrData();
	virtual ~StrData();
	virtual string *getFileName(void);

	void createFifo(void);
	void openFifo(void);
	void readFifo(Condition *c);

	void setFifoName(string *name);
protected:
	string m_filename;
};

#endif
