#include "Import.h"
#include "Feeder.h"
using namespace std;

Import::Import(Feeder *f) : Element(f)
{
}

Import::~Import()
{
}

bool Import::eval(void)
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
	m_feeder->getCurPos(&prev_ln, &prev_ch);

	if(! m_feeder->str(string("import")))
		return false;

	string tmp;
	m_feeder->blank(&tmp);

	if(! m_feeder->path(&m_path)){
		m_feeder->rewind(prev_ln,prev_ch);
		return false;
	}

	m_feeder->blank(&tmp);

	if(! m_feeder->str(string("as")))
		return false;

	m_feeder->blank(&tmp);

	if(! m_feeder->smallCaps(&m_prefix))
		return false;

	return true;
}

void Import::printOriginalString(void)
{
	cerr << "import " << m_path << " as " << m_prefix << endl;
}
