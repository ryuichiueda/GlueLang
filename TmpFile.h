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

class TmpFile : public Element
{
public:
	TmpFile(Feeder *f, Environment *env);
	virtual ~TmpFile();
	virtual bool parse(void);

	void setName(string s);

	virtual int exec(void);
	virtual bool eval(void);

	const char *actualFileName(void){return m_file_name.c_str();}
	const char *virtualFileName(void){return m_var_name.c_str();}

	int getFd(void){return m_fd;}

	bool m_append_mode;
protected:
	string m_var_name;
	string m_file_name;

	int m_fd;

	bool m_evaled;
};
#endif
