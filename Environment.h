#ifndef __ENVIRONMENT_H_
#define __ENVIRONMENT_H_

#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Environment
{
private:
	vector<string> m_args;
	map<string,string> m_variables;
	map<string, vector<string> > m_import_paths;
	vector<string> m_file_list;

	string m_dir;
public:
	Environment(int argc, char const* argv[],int script_pos);

	void setVariable(string *key, string *value);
	void appendValue(string *key, string *value);
	void getVariable(string *key, string *value);
	string *getArg(long pos);
	void getVariable(const char *key, string *value);

	void setImportPath(string *key, string *value);
	vector<string> *getImportPaths(string *key);
	vector<string> *getImportPaths(const char *key);

	void setFileList(string *filepath);
	void removeFiles(void);

	string m_error_msg;

	bool isImportPath(string *key);

	int getLevel(void){return m_level;}

	void init(void);

	int m_level;
	string m_glue_path;
	string m_tmpdir;

	bool m_v_opt;
	int m_pid;
};

#endif
