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

	bool getArg(string *ans);
	bool getComment(string *ans);
	bool getTmpFile(string *ans);

	void readAll(void);

	void close(void);
	void print(void);

	bool getCommand(string *ans);
	//bool ungetToken(string *str);

	bool atNewLine(void);

	bool atEnd(void);

	bool isAlphabet(char c);
	bool isNum(char c);
};
#endif
