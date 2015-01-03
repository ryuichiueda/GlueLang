#include "Element.h"
#include "Feeder.h"
#include "IfBlock.h"
#include "Import.h"
#include "Pipeline.h"
#include "CommandLine.h"
#include "Comment.h"
using namespace std;

IfBlock::IfBlock(Feeder *f, Environment *env) : Element(f,env)
{
}

IfBlock::~IfBlock()
{
}

/* IfBlock means an if-then-else chunk in usual programing languages.
 * ex)

| commandline1            <- if
	commandline2
| commandline2            <- else if
	commandline3
| otherwise               <- else
	commandline4

The other purpose to use IfBlock is the cancel of exit statuses

| seq '1' '1000000000' >>= head

Though the head command invokes SIGPIPE error of the seq command,
"|" cancels this failure in this case.
However, this technique should be carefully used.
 */
bool IfBlock::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(! m_feeder->str("|"))
		return false;

	while(1){
		// | commandline -> many (pipeline or commandline) -> | commandline ->...

		m_feeder->blank(NULL);
		
		if(add(new Pipeline(m_feeder,m_env))){
			((Pipeline *)m_nodes.back())->setIfFlag();
			break;
		}
		else if(add(new CommandLine(m_feeder,m_env))){
			((CommandLine *)m_nodes.back())->setIfFlag();
			break;
		}
		break;
	}
	m_feeder->getPos(&m_end_line, &m_end_char);

	if(!m_feeder->atEnd()){
		m_error_msg = "Unknown token";
		m_exit_status = 1;
		throw this;
	}

	return true;
}

int IfBlock::exec(void)
{
	int exit_status = 0;
	for(auto &c : m_nodes){
		exit_status = c->exec();
	}
	return 0;
}
