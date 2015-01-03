#include "Environment.h"
#include <fstream>
#include <iostream>
using namespace std;

Environment::Environment()
{
	m_level = 0;
}

void Environment::setImportPath(string *key, string *value)
{
	if(*key == "tmpdir"){
		m_import_paths[*key] = *value;
		return;
	}

	if(m_import_paths.find(*key) != m_import_paths.end()){
		m_error_msg = *key + " is already used" ;
		throw this;
	}

	m_import_paths[*key] = *value;
}

void Environment::getImportPath(string *key, string *value)
{
	if(m_import_paths.find(*key) == m_import_paths.end()){
		m_error_msg = *key + " not found" ;
		throw this;
	}

	*value = m_import_paths[*key];
}

bool Environment::isImportPath(string *key)
{
	return m_import_paths.find(*key) != m_import_paths.end();
}

void Environment::getImportPath(const char *key, string *value)
{
	string k = string(key);
	getImportPath(&k,value);
}

void Environment::setVariable(string *key, string *value)
{
	if(m_variables.find(*key) != m_variables.end()){
		m_error_msg = *key + " already exist" ;
		throw this;
	}

	m_variables[*key] = *value;
}

void Environment::getVariable(string *key,string *value)
{
	if(m_variables.find(*key) == m_variables.end()){
		m_error_msg = "variable " + *key + " not found";
		throw this;
	}

	*value = m_variables[*key];
}

void Environment::getVariable(const char *key,string *value)
{
	string k = string(key);
	return getVariable(&k,value);
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

