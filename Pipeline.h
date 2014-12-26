#ifndef __PIPELINE_H_
#define __PIPELINE_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Arg;
class CommandLine;

class Pipeline : public Element
{
public:
	Pipeline(Feeder *f);
	virtual ~Pipeline();
	virtual void print(int indent_level = 0);
	bool parse(void);

//	void setName(string s);
//	void appendArg(string a);

	virtual int exec(void);
protected:
	vector<CommandLine> m_commands;

	void execPipeline(void);

	string m_file_to_write;

	bool setRedirectTo(void);
};
#endif
