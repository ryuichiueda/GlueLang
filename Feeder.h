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

	bool literalEsc(string *ans);
	bool literalNoEsc(string *ans);

public:
	Feeder(ifstream *ifs);
	~Feeder();

	// procedures for handling the position
	void getPos(int *ln,int *ch);
	void setPos(int ln,int ch);

	int countIndent(void);

	bool literal(string *ans);
	// they scan the script, move the cursor
	// and return the string on matching
	bool variable(string *ans);
	bool arrayElem(string *name,long *pos);
	bool comment(void);
	bool declare(string *ans, string reserved);
	bool command(string *ans);
	bool blank(void);
	bool blankLine(void);
	bool path(string *ans);
	bool smallCaps(string *ans);
	bool positiveInt(long *pos);
	bool lineResidual(string *ans);

	bool str(string s);

	void readAll(void);

	void close(void);

	bool atNewLine(void);
	bool atEnd(void);
	bool isAlphabet(char c);
	bool isNum(char c);

	void debugOut(void);

	void printErrorPart(int from, int from_char, int to,int to_char);

	string m_error_msg;
};
#endif
