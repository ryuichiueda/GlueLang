#ifndef __FEEDER_H_
#define __FEEDER_H_

#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Feeder
{
private:
	ifstream* m_ifs;
	vector<string> m_lines;
	bool readLine(void);	

	int m_cur_line;
	int m_cur_char;
public:
	Feeder(ifstream *ifs);
	~Feeder();

	void readAll(void);

	void close(void);
	void print(void);

	bool getToken(string *p);
};
#endif
