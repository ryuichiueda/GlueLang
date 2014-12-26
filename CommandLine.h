#ifndef __COMMAND_LINE_H_
#define __COMMAND_LINE_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Arg;

class CommandLine : public Element
{
public:
	CommandLine(Feeder *f);
	virtual ~CommandLine();
	virtual void print(int indent_level = 0);
	bool parse(void);

	void setName(string s);
	void appendArg(string a);

	virtual int exec(void);
	virtual bool eval(void){return true;}

	virtual void printOriginalString(void);

	void setPipe(int *pip,int prev);
	int getPrevPipe(void){return m_pipe_prev;};
protected:
/*
	string m_name;
	vector<Arg> m_args;
*/

	void execCommandLine(void);

	//bool isCommandLine(string *str);

	string m_file_to_write;

	bool setRedirectTo(void);

	int m_pipe[2];
	int m_pipe_prev;
	bool m_is_piped;
};
#endif
