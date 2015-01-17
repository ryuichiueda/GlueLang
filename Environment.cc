#include "Environment.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/param.h> 
#include <sys/stat.h>
using namespace std;

Environment::Environment(int argc, char const* argv[],int script_pos)
{
	char *lv = getenv("GLUELEVEL");
	if(lv == NULL){
		m_level = 0;
	}else{
		char *e = NULL;
		m_level = (int)std::strtol(lv, &e, 10);
		if(errno != ERANGE && *e != '\0'){
			cerr << "***FATAL: STRANGE GLUELEVEL***" << endl;
			exit(1);
		}

		if(m_level >= INT_MAX || m_level < 0){
			cerr << "***FATAL: STRANGE GLUELEVEL***" << endl;
			exit(1);
		}
	}

	// set cwd (this directory is never changed)
	char tmp[MAXPATHLEN];
	if (getcwd(tmp,sizeof(tmp)) == NULL){
		cerr << "Initizalization error" << endl;
		perror ("getcwd");
		exit(1);
	}

	m_dir = tmp;	
	m_pid = getpid();

	string a0 = string(argv[0]);

	if(a0 == "glue")//from path
		m_glue_path = "/usr/local/bin/glue";
	else
		m_glue_path = string(tmp) + "/" + string(argv[0]);

	//set args
	for(int i=script_pos;i<argc;i++)
		m_args.push_back(argv[i]);

	m_v_opt = false;
}

// this function is called from Script::parse
// after import sentences are parsed.
void Environment::init(void)
{
	auto *p = getImportPaths("tmpdir");
	if(p == NULL || p->size() < 1){
		cerr << "unable to create tmpdir" << endl;
		throw this;
	}

	m_tmpdir = p->at(0) + "glue" + to_string(m_pid);
	while(mkdir(m_tmpdir.c_str(),0700) != 0){
		cerr << "unable to create tmpdir" << endl;
		throw this;
	}
}

/*
void Environment::setImportPath(const char *key,const char *value)
{
	m_import_paths[""].push_back(string(value));
}
*/

void Environment::setImportPath(string *key, string *value)
{
/*
	if(*value == ""){
		m_error_msg = "NULL path";
		throw this;
	}
*/

/*
	if(*key == "tmpdir"){
//		m_import_paths[*key] = *value;
		m_import_paths.insert(make_pair(*key,*value));
		return;
	}

	if(m_import_paths.find(*key) != m_import_paths.end()){
		m_error_msg = *key + " is already used" ;
		throw this;
	}
*/

	string path;
	if(value->at(0) == '/'){
		path = *value;
	}else{
		//resolve relative path (too lazy)
		path = m_dir + "/" + *value;
	}
	m_import_paths[*key].push_back(path);
}

vector<string> *Environment::getImportPaths(string *key)
{
	if(m_import_paths.find(*key) == m_import_paths.end()){
		return NULL;
	}

	return &m_import_paths[*key];
}

vector<string> *Environment::getImportPaths(const char *key)
{
	string tmp(key);
	return getImportPaths(&tmp);
}

bool Environment::isImportPath(string *key)
{
	return m_import_paths.find(*key) != m_import_paths.end();
}

/*
void Environment::getImportPath(const char *key, string *value)
{
	string k = string(key);
	getImportPath(&k,value);
}
*/

void Environment::setVariable(string *key, string *value)
{
	if(m_variables.find(*key) != m_variables.end()){
		m_error_msg = *key + " already exist" ;
		throw this;
	}

	m_variables[*key] = *value;
}

void Environment::appendValue(string *key, string *value)
{
	if(m_variables.find(*key) == m_variables.end()){
		m_error_msg = *key + " not exist" ;
		throw this;
	}

	m_variables[*key] += *value;
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
		if(m_v_opt)
			cerr << "+ pid " << getpid() << " file " << f << " deleted" << endl;
	}

	struct stat buf;
	while(stat(m_tmpdir.c_str(), &buf) == 0){
		if(remove(m_tmpdir.c_str()) != 0){
			cerr << "cannot remove tmpdir" << endl;
			sleep(1);
		}
	}
}

string *Environment::getArg(long pos)
{
	if(pos < 0 || pos >= (long)m_args.size()){
		m_error_msg = "Array index out of range (pos: "+
		to_string(pos) + ")";
		throw this;
	}
	return &m_args[pos];
}
