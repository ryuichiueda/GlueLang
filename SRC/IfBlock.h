#ifndef __IFBLOCK_H_
#define __IFBLOCK_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;

class IfBlock : public Element
{
public:
	IfBlock(Feeder *f, Environment *env);
	virtual ~IfBlock();
	//virtual void print(int indent_level = 0);
	bool parse(void);
	virtual int exec(void);
	virtual bool eval(void){return true;};
private:
	vector<bool> m_is_cond_node;
};
#endif
