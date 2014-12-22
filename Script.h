#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "Node.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class Script : public Node
{
public:
	Script(Feeder *f);
	virtual ~Script();
	//virtual void print(int indent_level = 0);
	bool parse(void);
	virtual int exec(void);
protected:
	vector<Node *> m_nodes;
};
#endif
