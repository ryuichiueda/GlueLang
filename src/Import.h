// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __IMPORT_H_
#define __IMPORT_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class Import : public Element
{
public:
	Import(Feeder *f, Environment *env,vector<int> *scopes);;
	virtual ~Import();

	virtual bool parse(void);

	bool parseAs(void);
	bool parsePATH(void);

protected:
	string m_path;
	string m_prefix;
};
#endif
