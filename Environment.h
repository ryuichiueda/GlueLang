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
	map<string,string> m_import_paths;
	vector<string> m_file_list;

	int m_level;

	string m_dir;
public:
	Environment(int argc, char const* argv[],int script_pos);

	void setVariable(string *key, string *value);
	void getVariable(string *key, string *value);
	string *getArg(long pos);
	void getVariable(const char *key, string *value);

	void setImportPath(string *key, string *value);
	void getImportPath(string *key, string *value);
	void getImportPath(const char *key, string *value);

	void setFileList(string *filepath);
	void removeFiles(void);

	string m_error_msg;

	bool isImportPath(string *key);

	void inclementLevel(void){m_level++;}
	int getLevel(void){return m_level;}
};

#endif
