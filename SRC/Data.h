// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __DATA_H_
#define __DATA_H_

#include <string>
using namespace std;

class Data
{
public:
	Data();
	virtual ~Data();

	virtual void setData(string *s){m_value = *s;}
	virtual string *getFileName(void) = 0;

	string m_error_msg;
	string m_value;
protected:
};

#endif
