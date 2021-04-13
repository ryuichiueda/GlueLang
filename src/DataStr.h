// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __STR_DATA_H_
#define __STR_DATA_H_

#include "Data.h"

class Data;

class DataStr : public Data
{
public:
	DataStr();
	virtual ~DataStr();
	virtual string *getFileName(void);

	void createFifo(void);
	void openFifo(void);
	void readFifo(void);

	void setFifoName(string *name);
protected:
	string m_filename;
	bool m_is_set;
};

#endif
