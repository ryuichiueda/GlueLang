#ifndef __ELEMENT_H_
#define __ELEMENT_H_

#include <vector>
#include <string>
using namespace std;

class Feeder;

class Element
{
protected:
	Feeder *m_feeder;

	vector<string> m_error_messages;
public:
	Element(Feeder *f);
	virtual ~Element();
	virtual void printErrorMessages(void);
	virtual int exec(void) = 0;

/*
	void setFileList(string *filepath);
	void removeFiles(void);
*/
};
#endif
