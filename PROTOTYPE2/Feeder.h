#ifndef __FEEDER_H_
#define __FEEDER_H_

#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Feeder
{
private:
	ifstream * m_ifs;
	vector<string> m_lines;
public:
	Feeder(ifstream *ifs);
	~Feeder();

	bool readLine(void);	

	void print(void);
};
#endif
