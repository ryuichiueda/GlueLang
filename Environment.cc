#include "Environment.h"
#include <fstream>
#include <iostream>
using namespace std;

bool Environment::setImportPath(string *key, string *value)
{
	if(*key == "tmpdir"){
		m_import_paths[*key] = *value;
		return true;
	}

	if(m_import_paths.find(*key) != m_import_paths.end())
		return false;

	m_import_paths[*key] = *value;
	return true;
}

bool Environment::getImportPath(string *key, string *value)
{
	if(m_import_paths.find(*key) == m_import_paths.end())
		return false;

	*value = m_import_paths[*key];
	return true;
}

bool Environment::getImportPath(const char *key, string *value)
{
	string k = string(key);
	return getImportPath(&k,value);
/*
	if(m_import_paths.find(k) == m_import_paths.end())
		return false;

	*value = m_import_paths[k];
	return true;
*/
}

bool Environment::setVariable(string *key, string *value)
{
	if(m_variables.find(*key) != m_variables.end())
		return false;

	m_variables[*key] = *value;
	return true;
}

bool Environment::getVariable(string *key,string *value)
{
	if(m_variables.find(*key) == m_variables.end())
		return false;

	*value = m_variables[*key];
	return true;
}

bool Environment::getVariable(const char *key,string *value)
{
	string k = string(key);
	return getVariable(&k,value);
/*
	if(m_variables.find(k) == m_variables.end())
		return false;

	*value = m_variables[k];
	return true;
*/
}

void Environment::setFileList(string *filepath)
{
	m_file_list.push_back(*filepath);
}

void Environment::removeFiles(void)
{
	for(auto f : m_file_list){
		remove(f.c_str());
	}
}

void Environment::debug(void)
{
}
