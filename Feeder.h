#ifndef __FEEDER_H_
#define __FEEDER_H_

#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Feeder
{
private:
	ifstream* m_ifs;
	vector<string> m_lines;
	bool readLine(void);	

	int m_cur_line;
	int m_cur_char;

	map<string,string> m_variables;
	vector<string> m_file_list;
public:
	Feeder(ifstream *ifs);
	~Feeder();

	bool literalString(string *ans);
	bool variable(string *ans);
	bool comment(string *ans);
	bool tmpFile(string *ans);
	bool command(string *ans);
	bool blank(string *ans);

	void readAll(void);

	void close(void);
	void print(void);

	void debugOut(void);

	//bool ungetToken(string *str);

	bool atNewLine(void);

	bool atEnd(void);

	bool isAlphabet(char c);
	bool isNum(char c);

	bool setVariable(string *key, string *value);
	bool getVariable(string *key, string *value);

	void setFileList(string *filepath);
	void removeFiles(void);
};
#endif
