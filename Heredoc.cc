#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "CommandLine.h"
#include "TmpFile.h"
#include "VarString.h"
#include "Pipeline.h"
#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
#include "Heredoc.h"
#include "Where.h"
using namespace std;

Heredoc::Heredoc(Feeder *f, Environment *env) : Element(f,env)
{
}

Heredoc::~Heredoc()
{
}

void Heredoc::print(int indent_level)
{
}

/*
 * A here document is written like this:
 * file <name> = '''
 * 	<content>
 * '''
 * An indent is required at each line of the content.
 * The length of the indent is determined as
 * the shortest one in those of content lines.
 */
bool Heredoc::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(! add(new TmpFile(m_feeder,m_env))){
		return false;
	}

	if(! m_feeder->str("'''")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

bool Heredoc::eval(void)
{
	return true;
}

int Heredoc::exec(void)
{
	return 0;
}

