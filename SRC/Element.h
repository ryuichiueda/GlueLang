// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __ELEMENT_H_
#define __ELEMENT_H_

#include <vector>
#include <string>
using namespace std;

class Feeder;
class Environment;
class DefFile;
class DefStr;

class Element
{
protected:
	Feeder *m_feeder;
	Environment *m_env;

	vector<Element *> m_nodes;

	int m_start_line, m_start_char;
	int m_end_line, m_end_char;

	int m_job_id;
public:
	Element(Feeder *f, Environment *env);
	virtual ~Element();
	virtual bool eval(void){return true;}
	virtual int exec(void){return 0;}
	virtual bool parse(void){return false;}

	virtual void printOriginalString(void);

	virtual void setJobId(int id);

	bool add(Element *n);

	string m_error_msg;
	int m_exit_status;
	bool m_command_error;

	string pos(void);
	int getExitStatus(void){return m_exit_status;};
	void printErrorPart(void);
	int getLevel(void);

	DefFile *m_outfile;
	DefStr *m_outstr;

	static int m_signal;
};

#endif
