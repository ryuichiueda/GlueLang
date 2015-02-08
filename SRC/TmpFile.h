#ifndef __TMP_FILE_H_
#define __TMP_FILE_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;
class Arg;
class FileData;

class TmpFile : public Element
{
public:
	TmpFile(Feeder *f, Environment *env);
	virtual ~TmpFile();
	virtual bool parse(void);
	virtual int exec(void);

	void setName(string s);


	const char *actualFileName(void){return m_file_name.c_str();}
	const char *virtualFileName(void){return m_var_name.c_str();}

	bool m_append_mode;

	string m_var_name;
	string m_file_name;
protected:
	FileData *m_data;
};
#endif
