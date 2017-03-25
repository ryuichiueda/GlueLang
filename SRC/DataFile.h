// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __FILE_DATA_H_
#define __FILE_DATA_H_

#include "Data.h"

class Data;

class DataFile : public Data
{
public:
	DataFile();
	virtual ~DataFile();
	virtual string *getFileName(void);

	void openFile(void);
	void setAppend(void);
private:
	bool m_append_mode;
};

#endif
