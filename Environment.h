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
	map<string,string> m_variables;
	map<string,string> m_import_paths;
	vector<string> m_file_list;

	int m_level;
public:
	Environment();
	void setVariable(string *key, string *value);
	void getVariable(string *key, string *value);
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
