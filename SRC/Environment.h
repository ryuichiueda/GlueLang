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
	vector<string> m_args;
	map<string, vector<string> > m_import_paths;

	string m_dir;

	map<string,Data *> m_data;
public:
	void setData(string *key, Data *value);

	Environment(int argc, char const* argv[],int script_pos);
	~Environment();
	void initSubShell(char const* argv[]);

	//void setVariable(string *key, string *value);
	void appendValue(string *key, string *value);
	void getVariable(string *key, string *value);
	Data *getData(string *key);
	string *getArg(long pos);
	//void getVariable(const char *key, string *value);

	void setImportPath(string *key, string *value);
	vector<string> *getImportPaths(string *key);
	vector<string> *getImportPaths(const char *key);

	//void setFileList(string *filepath);
	void removeFiles(void);

	string m_error_msg;

	bool isImportPath(string *key);

	int getLevel(void){return m_level;}

	void initTmpdir(void);

	int m_level;
	string m_tmpdir;

	bool m_v_opt;
	int m_pid;
};

#endif
