#include "Comment.h"
#include "Feeder.h"
#include "Environment.h"
#include <iostream>
using namespace std;

Comment::Comment(Feeder *f, Environment *env) : Element(f,env)
{
}

Comment::~Comment()
{
}

bool Comment::parse(void)
{
	return m_feeder->comment(&m_text);
}

void Comment::printOriginalString(void)
{
	cerr << m_text << "\n";
}
