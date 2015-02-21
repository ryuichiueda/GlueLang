#ifndef __ENVIRONMENT_H_
#define __ENVIRONMENT_H_

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "Data.h"
using namespace std;

class Environment
{
private:
	vector<string> m_args; //args of this command
	map<string, vector<string> > m_import_paths;
	string m_dir;
	map<string,Data *> m_data;

	int m_job_counter;
public:
	void setData(string *key, Data *value);

	Environment(int argc, char const* argv[],int script_pos);
	Environment(Environment *e);
	~Environment();

	// fork
	void initExeProc(char const* argv[]);
	int getLevel(void){return m_level;}
	int m_level;

	// manipulators for m_data
	Data *getData(string *key);
	bool isData(string *key);
	string *getArg(long pos);

	// manipulators for m_import_paths
	void setImportPath(string *key, string *value);
	vector<string> *getImportPaths(string *key);
	vector<string> *getImportPaths(const char *key);
	bool isImportPath(string *key);

	void removeFiles(bool local);

	void initTmpdir(void);

	string m_error_msg;
	string m_tmpdir;
	bool m_v_opt;
	int m_pid;

	map<int, Environment *> m_local_env;

	// giving job id
	int publishJobId(void);
};

#endif
