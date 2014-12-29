#ifndef __ELEMENT_H_
#define __ELEMENT_H_

#include <vector>
#include <string>
using namespace std;

class Feeder;
class Environment;

class Element
{
protected:
	Feeder *m_feeder;
	Environment *m_env;

	vector<string> m_error_messages;
	vector<Element *> m_nodes;
public:
	Element(Feeder *f, Environment *env);
	virtual ~Element();
	//virtual void printErrorMessages(void);
	virtual bool eval(void) = 0;
	virtual int exec(void) = 0;
	virtual bool parse(void) = 0;

	virtual void printOriginalString(void);

	bool add(Element *n);

	void errorCheck(void);
};
#endif
