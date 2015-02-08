#include "Environment.h"
#include "Element.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h> 
#include <sys/stat.h>
#include <signal.h>
#include "JobData.h"
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

	//set args
	for(int i=script_pos;i<argc;i++)
		m_args.push_back(argv[i]);

	m_v_opt = false;
	m_level = 0;
}

Environment::~Environment()
{
	for(auto d : m_data){
		if(d.second != NULL)
			delete d.second;
	}
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

Data *Environment::getData(string *key)
{
	if(m_data.find(*key) == m_data.end()){
		m_error_msg = "variable " + *key + " not found";
		throw this;
	}

	return m_data[*key];
}

void Environment::removeFiles(void)
{
	for(auto d : m_data){
		string *f = d.second->getFileName();
		if(f == NULL)
			continue;

		if(m_tmpdir + "/" != f->substr(0,m_tmpdir.size() + 1))
			continue;

		remove(f->c_str());
		if(m_v_opt)
			cerr << "+ pid " << getpid() << " file " << f << " deleted" << endl;
	}
	
/*
	for(auto f : m_file_list){
		if(m_tmpdir + "/" != f.substr(0,m_tmpdir.size() + 1))
			continue;

		remove(f.c_str());
		if(m_v_opt)
			cerr << "+ pid " << getpid() << " file " << f << " deleted" << endl;
	}
*/

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

void Environment::setData(string *key, Data *value)
{
	if(m_data.find(*key) != m_data.end()){
		m_error_msg = *key + " already exist";
		throw this;
	}
	m_data[*key] = value;
}
