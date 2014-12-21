#ifndef __NODE_H_
#define __NODE_H_

#include <vector>
#include <string>
using namespace std;

class Feeder;

class Node
{
protected:
	Feeder *m_feeder;
//	vector<Node *> m_node;
//	bool m_set;

	vector<string> m_error_messages;
public:
	Node(Feeder *f);
	virtual ~Node();
//	virtual void print(int indent_level = 0);
//	virtual void parser(vector<char> *script,int pos) = 0;
/*
	Node *getNode(int at);
	void pushNode(Node *n);

	bool isSet(void){return m_set;}
*/

	virtual bool isNum(char c);
	virtual bool isAlphabet(char c);
};
#endif
