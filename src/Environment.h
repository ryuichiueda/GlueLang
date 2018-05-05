// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
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

	string genKey(int scope_id,string *key);
	Data *getData(int scope_id,string *key);

	int m_job_counter;
public:
	void setData(int scope_id,string *key, Data *value);

	Environment(int argc, char const* argv[],int script_pos);
//	Environment(Environment *e);
	~Environment();

	// fork
	void initExeProc(char const* argv[]);
	int getLevel(void){return m_level;}
	int m_level;

	// manipulators for m_data
	Data *getData(vector<int> *scopes,string *key);
	bool isData(int scope_id,string *key);
	string *getArg(long pos);

	// manipulators for m_import_paths
	void setImportPath(string *key, string *value);
	vector<string> *getImportPaths(string *key);
	vector<string> *getImportPaths(const char *key);
	bool isImportPath(string *key);

	void removeFiles(void);

	void initTmpdir(void);

	string m_error_msg;
	string m_tmpdir;
	bool m_v_opt;
	int m_pid;
	int m_ppid;

	// giving job id
	int publishJobId(void);

	//bool m_is_local;
};

#endif
