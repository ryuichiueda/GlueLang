#include "Element.h"
#include "Feeder.h"
#include "IfBlock.h"
#include "Import.h"
#include "Pipeline.h"
#include "Job.h"
#include "CommandLine.h"
using namespace std;

IfBlock::IfBlock(Feeder *f, Environment *env) : Element(f,env)
{
}

IfBlock::~IfBlock()
{
}

/* IfBlock means an if-then-else chunk in usual programing languages.
 * ex)

? commandline1	    <- if
	commandline2
| commandline2	    <- else if
	commandline3
| otherwise	       <- else
	commandline4
			 
commandline5	      <- out of if block

The other purpose to use IfBlock is the cancel of exit statuses

? seq '1' '1000000000' >>= head

Though the head command invokes SIGPIPE error of the seq command,
"|" cancels this failure in this case.
However, this technique should be carefully used.
 */
bool IfBlock::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	int indent = m_feeder->countIndent();
	
	m_feeder->blank();
	// detect the first condition
	if(! m_feeder->str("?")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	while(1){
		// condition -> many (pipeline or commandline) -> condition ->...
		if(m_feeder->str("otherwise")){
			m_nodes.push_back(NULL); // dummy
		}else if(add(new Job(m_feeder,m_env))){
			((Job *)m_nodes.back())->m_if = true;
		}else{
			m_feeder->setPos(m_start_line, m_start_char);
			return false;
		}

		while(m_feeder->blankLine()){}

		m_is_cond_node.push_back(true);

		int sub_indent = m_feeder->countIndent();
		// end of the if block
		if(sub_indent < indent)
			break;

		// found another condition
		if( indent == sub_indent){
			m_feeder->blank();
			if(m_feeder->str("?")){
				// another condition
				continue;
			}else{
				break; // end of the if block
			}
		}

		// attempt to search procedures related to the condition
		while(sub_indent > indent){
			m_feeder->blank();

			if(add(new IfBlock(m_feeder,m_env))
				|| add(new Job(m_feeder,m_env))){

				m_is_cond_node.push_back(false);
				while(m_feeder->blankLine()){}
				sub_indent = m_feeder->countIndent();
			}else{
				m_feeder->setPos(m_start_line, m_start_char);
				return false;
			}
		}

		// after procedures related to the condition
		if(sub_indent < indent){
			m_feeder->getPos(&m_end_line, &m_end_char);
			break;
		}

		m_feeder->getPos(&m_end_line, &m_end_char);
		// next condition
		m_feeder->blank();
		// detect the first condition
		if(! m_feeder->str("|")){
			m_feeder->setPos(m_end_line, m_end_char);
			break;
		}
	}
	m_feeder->getPos(&m_end_line, &m_end_char);

	return true;
}

int IfBlock::exec(void)
{
	if(m_nodes.size() != m_is_cond_node.size()){
		m_error_msg = "BUG: IfBlock internal error";
		throw this;
	}

	int exit_status = 0;
	bool skip = false;
	bool match = false;
	for(int i=0;i< (int)m_nodes.size();i++){
		if(m_is_cond_node[i]){ // cond node
			if(match) // already matched
				break;	
			
			if(m_nodes[i] == NULL || m_nodes[i]->exec() == 0){
				match = true;
				skip = false;
			}else
				skip = true;

		}else{ // other node
			if(!skip)
				m_nodes[i]->exec();
		}
	}
/*
	for(auto &c : m_nodes){
	}
*/
	return exit_status;
}
