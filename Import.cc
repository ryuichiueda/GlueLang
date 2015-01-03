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
	int prev_ln,prev_ch;
	m_feeder->getPos(&prev_ln, &prev_ch);

	if(! m_feeder->str(string("import")))
		return false;

	m_feeder->blank(NULL);

	if(! m_feeder->path(&m_path)){
		m_feeder->setPos(prev_ln,prev_ch);
		return false;
	}

	m_feeder->blank(NULL);

	if(! m_feeder->str(string("as")))
		return false;

	m_feeder->blank(NULL);

	if(! m_feeder->smallCaps(&m_prefix))
		return false;

	//evaluate beforehand
	return m_env->setImportPath(&m_prefix,&m_path);
}

void Import::printOriginalString(void)
{
	cerr << "import " << m_path << " as " << m_prefix << endl;
}
