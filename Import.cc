#include "Import.h"
#include "Feeder.h"
#include "Environment.h"
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

	m_feeder->blank(NULL);

	if(! m_feeder->path(&m_path)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_feeder->blank(NULL);

	if(! m_feeder->str(string("as")))
		return false;

	m_feeder->blank(NULL);

	if(! m_feeder->smallCaps(&m_prefix))
		return false;

	m_feeder->getPos(&m_end_line, &m_end_char);

	//evaluate beforehand
	try{
		m_env->setImportPath(&m_prefix,&m_path);
	}
	catch(...){
		m_error_msg = "Import error: " + m_env->m_error_msg;
		m_exit_status = 1;
		throw this;
	}

	return true;
}

void Import::printOriginalString(void)
{
	cerr << "import " << m_path << " as " << m_prefix << endl;
}
