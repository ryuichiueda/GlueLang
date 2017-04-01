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
	vector<int> m_scopes;
public:
	Element(Feeder *f, Environment *env, vector<int> *scopes);
	virtual ~Element();
	virtual bool eval(void){return true;}
	virtual int exec(DefFile *f, DefFile *ef, DefStr *s){return 0;}
	virtual bool parse(void){return false;}

	virtual void printOriginalString(void);

	virtual void setJobId(int id);

	bool add(Element *n);

	string m_error_msg;
	int m_exit_status;
	bool m_command_error; //It becomes true when a command returns an error.

	string pos(void);
	int getExitStatus(void){return m_exit_status;};
	void printErrorPart(void);
	int getLevel(void);

	static int m_signal;
};

#endif
