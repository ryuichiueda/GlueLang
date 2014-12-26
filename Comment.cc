#include "Comment.h"
#include "Feeder.h"
#include <iostream>
using namespace std;

Comment::Comment(Feeder *f) : Element(f)
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
