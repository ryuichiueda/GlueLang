#ifndef __FEEDER_H_
#define __FEEDER_H_

#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Feeder
{
private:
	ifstream* m_ifs; // related to the script file
	vector<string> m_lines; // in which the script is stored

	bool readLine(void);

	// cursor for pointing the position of scanning
	int m_cur_line;
	int m_cur_char;

	bool outOfRange(void);
	void checkEol(string *p);
public:
	Feeder(ifstream *ifs);
	~Feeder();

	// procedures for handling the position
	void getPos(int *ln,int *ch);
	void setPos(int ln,int ch);

	// they scan the script, move the cursor
	// and return the string on matching
	bool literalString(string *ans);
	bool variable(string *ans);
	bool comment(string *ans);
	bool declare(string *ans, string reserved);
	bool command(string *ans);
	bool blank(string *ans);
	bool pipe(string *ans);
	bool path(string *ans);
	bool import(string *ans);
	bool smallCaps(string *ans);

	bool str(string s);

	void readAll(void);

	void close(void);

	bool atNewLine(void);
	bool atEnd(void);
	bool isAlphabet(char c);
	bool isNum(char c);

	void debugOut(void);

	void printErrorPart(int from, int from_char, int to,int to_char);
};
#endif
