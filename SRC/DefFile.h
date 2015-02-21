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
class DataFile;

class DefFile : public Element
{
public:
	DefFile(Feeder *f, Environment *env);
	virtual ~DefFile();
	virtual bool parse(void);
	virtual int exec(void);

	DataFile *m_data;
};
#endif
