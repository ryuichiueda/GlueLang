#include "Comment.h"
#include "Feeder.h"
#include <iostream>
using namespace std;

Comment::Comment(Feeder *f) : Node(f)
{
}

Comment::~Comment()
{
}

bool Comment::parse(void)
{
	return m_feeder->getComment(&m_text);
}
