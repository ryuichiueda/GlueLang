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

	vector<string> m_error_messages;
public:
	Node(Feeder *f);
	virtual ~Node();
	virtual void printErrorMessages(void);
	virtual int exec(void) = 0;

/*
	void setFileList(string *filepath);
	void removeFiles(void);
*/
};
#endif
