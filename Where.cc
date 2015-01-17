#include "Element.h"
#include "Feeder.h"
#include "Where.h"
#include "Import.h"
#include "Procedure.h"
#include "IfBlock.h"
#include "Pipeline.h"
#include "AndLine.h"
#include "CommandLine.h"
#include "Environment.h"
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
using namespace std;

Where::Where(Feeder *f, Environment *env) : Element(f,env)
{
}

Where::~Where()
{
}

bool Where::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

int Where::exec(void)
{
	return 0;
}
