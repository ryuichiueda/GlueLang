#ifndef __ELEMENT_H_
#define __ELEMENT_H_

#include <vector>
#include <string>
using namespace std;

class Feeder;
class Environment;
class TmpFile;
class VarString;

class Element
{
protected:
	Feeder *m_feeder;
	Environment *m_env;

	vector<Element *> m_nodes;

	int m_start_line, m_start_char;
	int m_end_line, m_end_char;
	int m_exit_status;
public:
	Element(Feeder *f, Environment *env);
	virtual ~Element();
	virtual bool eval(void) = 0;
	virtual int exec(void) = 0;
	virtual bool parse(void) = 0;

	virtual void printOriginalString(void);

	bool add(Element *n);

	string m_error_msg;

	string pos(void);
	int getExitStatus(void){return m_exit_status;};
	void printErrorPart(void);
	int getLevel(void);

	TmpFile *m_outfile;
	VarString *m_outstr;
};
#endif
