#include "Import.h"
#include "Feeder.h"
#include "Environment.h"
#include <string.h>
using namespace std;

Import::Import(Feeder *f, Environment *env) : Element(f,env)
{
}

Import::~Import()
{
}

bool Import::eval(void)//nothing to do here
{
	return true;
}

int Import::exec(void)
{
	return 0;
}

bool Import::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(! m_feeder->str(string("import")))
		return false;

	if(! m_feeder->path(&m_path)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	if(m_feeder->str(string("as")))
		return parseAs();

	//import PATH
	if(m_path == "PATH"){
		return parsePATH();
	}
	
	m_feeder->setPos(m_start_line, m_start_char);
	return false;
}

bool Import::parsePATH(void)
{
	char *str = getenv("PATH");

	if(str == NULL){
		m_feeder->setPos(m_start_line,m_start_char);
		return false;
	}

	string nokey("");

	char *p = strtok(str,":");
	while(p != NULL){
		string path = string(p) + string("/");
		m_env->setImportPath(&nokey,&path);
		p = strtok(NULL,":");
	}

	m_feeder->getPos(&m_end_line,&m_end_char);
	return true;
}

bool Import::parseAs(void)
{

	if(! m_feeder->smallCaps(&m_prefix)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	//evaluate beforehand
	try{
		m_env->setImportPath(&m_prefix,&m_path);
	}
	catch(...){
		m_error_msg = "Import error: " + m_env->m_error_msg;
		m_exit_status = 1;
		m_feeder->getPos(&m_end_line, &m_end_char);
		throw this;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void Import::printOriginalString(void)
{
	cerr << "import " << m_path << " as " << m_prefix << endl;
}
