#ifndef __FILE_DATA_H_
#define __FILE_DATA_H_

#include "Data.h"

class Data;

class FileData : public Data
{
public:
	FileData();
	virtual ~FileData();
	virtual string *getFileName(void);

	void openFile(void);

	void setAppend(void);

private:
	bool m_append_mode;
};

#endif
