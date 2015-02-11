#ifndef __ARG_EXT_COM_H_
#define __ARG_EXT_COM_H_

#include "Arg.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class ArgExtCommand : public Arg
{
public:
	ArgExtCommand(Feeder *f,Environment *env);
	virtual ~ArgExtCommand();

	//virtual bool eval(void);
	virtual bool parse(void);
private:
	bool parsePrefixedCom(void);
	string m_prefix;
};
#endif
