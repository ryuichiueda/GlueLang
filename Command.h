#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "Node.h"
#include <iostream>
#include <vector>
using namespace std;

class Node;
class Feeder;
class Arg;

class Command : public Node
{
public:
	Command(Feeder *f);
	virtual ~Command();
	virtual void print(int indent_level = 0);
	bool parse(void);

	void setName(string s);
	void appendArg(string a);

	virtual int exec(void);
protected:
	string m_name;
	vector<Arg> m_args;

	void execCommand(void);

	//bool isCommand(string *str);

	string m_file_to_write;

	bool setRedirectTo(void);
};
#endif
