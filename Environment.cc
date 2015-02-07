#include "Environment.h"
#include "Element.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h> 
#include <sys/stat.h>
#include <signal.h>
using namespace std;

Environment::Environment(int argc, char const* argv[],int script_pos)
{
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

/*
	if(a0 == "glue")//from path
		m_glue_path = "/usr/local/bin/glue";
	else
		m_glue_path = string(tmp) + "/" + string(argv[0]);
*/

	//set args
	for(int i=script_pos;i<argc;i++)
		m_args.push_back(argv[i]);

	m_v_opt = false;
}

void Environment::initSubShell(char const* argv[])
{
	m_pid = getpid();
	//set args
	int c = 0;
	m_args.clear();
	while(argv[c] != NULL){
		m_args.push_back(argv[c]);
		c++;
	}
}

// this function is called from Script::parse
// after import sentences are parsed.
void Environment::initTmpdir(void)
{
	auto *p = getImportPaths("tmpdir");
	if(p == NULL || p->size() < 1){
		cerr << "unable to find tmpdir" << endl;
		throw this;
	}

	srand(time(NULL));

	m_tmpdir = p->at(0) + "glue" + to_string(m_pid) + "-" + to_string(rand());
	while(mkdir(m_tmpdir.c_str(),0700) != 0){
		cerr << "unable to create tmpdir" << endl;
		throw this;
	}
}


void Environment::setImportPath(string *key, string *value)
{
	string path;
	if(value->at(0) == '/'){
		path = *value;
	}else{
		//resolve relative path (too lazy)
		path = m_dir + "/" + *value;
	}
	m_import_paths[*key].push_back(path);
}

bool Environment::initBG(string *key)
{
	if(m_background_jobs.find(*key) != m_background_jobs.end())
		return false;

	m_background_jobs[*key] = 0;
	return true;
}

bool Environment::setBG(string *key, int value)
{
	if(m_background_jobs.find(*key) == m_background_jobs.end())
		return false;

	m_background_jobs[*key] = value;
	return true;
}

void Environment::unsetBG(const char *key)
{
	string k(key);
	m_background_jobs.erase(k);
}

int Environment::getBG(const char *key)
{
	string k(key);
	if(m_background_jobs.find(k) == m_background_jobs.end()){
		return -1;
	}

	return m_background_jobs[k];
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
		if(m_tmpdir + "/" != f.substr(0,m_tmpdir.size() + 1))
			continue;

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
	if(Element::m_signal != 0)
		kill(0,SIGHUP);
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
