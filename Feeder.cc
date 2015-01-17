#include "Feeder.h"
#include <fstream>
#include <iostream>
#include <limits.h>
#include <string.h>
using namespace std;

Feeder::Feeder(ifstream *ifs){
	if (ifs->fail()){
		cerr << "glue: script read error" << endl;
//		return EXIT_FAILURE;
	}
	m_ifs = ifs;
	m_cur_line = 0;
	m_cur_char = 0;

	readAll();
}

Feeder::~Feeder()
{
}

bool Feeder::readLine(void)
{
	string str;
	if(!getline(*m_ifs, str))
		return false;
	m_lines.push_back(str);
	return true;
}

void Feeder::close(void)
{
	if(m_ifs == NULL)
		return;

	m_ifs->close();
	m_ifs = NULL;
}

bool Feeder::command(string *ans)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;
	for(;i < (int)p->length();i++){
		char c = p->at(i);
		if(strchr(" #",c) != NULL)
			break;

		if(! isAlphabet(c) && ! isNum(c) && strchr("._-/",c) == NULL)
			return false;
	}
	if(i == m_cur_char)
		return false;

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i;
	blank();
	checkEol(p);
	return true;
}

bool Feeder::smallCaps(string *ans)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];

	int i = m_cur_char;
	for(;i < (int)p->length();i++){
		if( p->at(i) < 'a' || p->at(i) > 'z')
			break;
	}

	if(i == m_cur_char)
		return false;

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i;
	checkEol(p);
	return true;
}
bool Feeder::blankLine(void)
{
	if(outOfRange() || m_cur_char != 0)
		return false;
	
	string *p = &m_lines[m_cur_line];
	for(auto c : *p){
		if(strchr(" \t",c) == NULL)
			return false;
	}

	m_cur_char = p->size();
	checkEol(p);
	return true;
}

bool Feeder::blank(void)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];

	if(p->size() - m_cur_char < 1)
		return false;

	int i = m_cur_char;
	if(strchr(" \t",p->at(i)) == NULL)
		return false;

	for(;i < (int)p->length();i++){
		if(strchr(" \t",p->at(i)) == NULL)
			break;
	}

	m_cur_char = i;
	checkEol(p);
	return true;
}

bool Feeder::path(string *ans)
{
	if(command(ans)){
		blank();
		return true;
	}
	return false;
}

bool Feeder::arrayElem(string *name,long *pos)
{
	if(outOfRange())
		return false;

	int bl = m_cur_line;
	int bc = m_cur_char;

	if(!variable(name))
		return false;

	if(atNewLine()){
		setPos(bl,bc);
		return false;
	}

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;
	if(p->at(i) != '['){
		setPos(bl,bc);
		return false;
	}

	m_cur_char++;
	checkEol(p);

	blank();

	if(!positiveInt(pos)){
		// It must be a positive integer after '['
		// in this stage. (A variable should be permitted
		// in future)
		m_error_msg = "Invalid array pos";
		throw this;
	}

	blank();

	p = &m_lines[m_cur_line];
	if(p->at(m_cur_char) != ']'){
		setPos(bl,bc);
		return false;
	}

	m_cur_char++;
	checkEol(p);
	return true;
}

bool Feeder::variable(string *ans)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;
	if(p->at(i) == '>')
		return false;

	for(;i < (int)p->length();i++){
		char c = p->at(i);
		if(strchr("> #[",c))
			break;

		if(! isAlphabet(c) && ! isNum(c) && c != '_' && c != '-')
			return false;
	}

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i;
	blank();
	checkEol(p);
	return true;
}

bool Feeder::literal(string *ans)
{
	return literalNoEsc(ans) || literalEsc(ans);
}

// an arg that starts from - (a hyphen)
bool Feeder::literalNoEsc(string *ans)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;

	if(p->at(i) != '-' && ! isNum(p->at(i)))
		return false;

	i++;

	for(;i < (int)p->length();i++){
		if(strchr(" \t",p->at(i)))
			break;
	}

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i;
	blank();
	checkEol(p);
	return true;
}

// An arg whose initial character is '-' or '0-9'
// should be a string that is quoted by '.
// This function gives escaped characters as
// they are escaped.
bool Feeder::literalEsc(string *ans)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;

	if(p->at(i) != '\'')
		return false;

	i++;

	bool escaped = false;

	for(;i < (int)p->length();i++){
		char c = p->at(i);
		if(c == '\\'){
			escaped = true;
			continue;
		}

		if(c == '\''){
			if(escaped){
				escaped = false;
				continue;
			}else{
				break;
			}
		}else{
			escaped = false;
		}

	}

	*ans = string(p->c_str()+m_cur_char+1,i-m_cur_char-1);
	m_cur_char = i+1;
	blank();
	checkEol(p);
	return true;
}

void Feeder::readAll(void)
{
	while(readLine()){};
}

bool Feeder::atNewLine(void)
{
	return m_cur_char == 0;
}

bool Feeder::atEnd(void)
{
	return (m_cur_line == (int)m_lines.size()) && m_cur_char == 0;
}

bool Feeder::outOfRange(void)
{
	return (m_cur_line >= (int)m_lines.size());
}

/* comment = blank + blankLines + # + strings + blankLines */
bool Feeder::comment(void)
{
	int ln,ch;
	getPos(&ln,&ch);

	blank();
	while(blankLine()){ };
	if(outOfRange())
		return false;

	if(atEnd())
		return true;

	string *p = &m_lines[m_cur_line];
	if((int)p->size() <= m_cur_char || p->at(m_cur_char) != '#'){
		setPos(ln,ch);
		return false;
	}

	m_cur_char++;

	lineResidual(NULL);
	while(blankLine()){ };
	return true;
}

bool Feeder::positiveInt(long *pos)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;

	if(! isNum(p->at(i)))
		return false;

	i++;
	for(;i < (int)p->length();i++){
		if(! isNum(p->at(i)) )
			break;
	}

	string str = string(p->c_str()+m_cur_char,i-m_cur_char);
	char *e = NULL;
	*pos = strtol(str.c_str(), &e, 10);

	if(errno != ERANGE && *e != '\0'){
		m_error_msg = "Invalid number";
		throw this;
	}
	else if (*pos == LONG_MAX){
		m_error_msg = "Too large number";
		throw this;
	}

	m_cur_char = i;
	checkEol(p);
	return true;
}

bool Feeder::str(string s)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	if(p->substr(m_cur_char,s.size()) != s)
		return false;

	m_cur_char += s.length();
	blank();
	checkEol(p);
	return true;
}

// <reserved> variable = ...
bool Feeder::declare(string *ans, string reserved)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];

	int len = reserved.size();

	if(p->substr(0,len+1) != reserved + " ")
		return false;

	int i = m_cur_char + len + 1;
	for( ; i < (int)p->length() ;i++){
		if( ! isAlphabet(p->at(i)) )
			break;
	}
	int last = i;
	while(strchr(" \t",p->at(i)) != NULL)
		i++;

	if(p->at(i++) != '=')
		return false;

	while(p->at(i) == ' ' || p->at(i) == '\t')
		i++;

	*ans = string(p->c_str()+m_cur_char+len+1,last-m_cur_char-len-1);
	m_cur_char = i;
	checkEol(p);
	return true;
}

void Feeder::checkEol(string *p)
{
	if(m_cur_char < (int)p->length())
		return;

	m_cur_char = 0;
	m_cur_line++;
}

bool Feeder::isAlphabet(char c)
{
	if(c >= 'a' && c <= 'z')
		return true;
	if(c >= 'A' && c <= 'Z')
		return true;

	return false;
}

bool Feeder::isNum(char c)
{
	return c >= '0' && c <= '9';
}

void Feeder::debugOut(void)
{
	cerr << m_cur_line << " " << m_cur_char << endl;
}

void Feeder::getPos(int *ln,int *ch)
{
	*ln = m_cur_line;
	*ch = m_cur_char;
}

void Feeder::setPos(int ln,int ch)
{
	m_cur_line = ln;
	m_cur_char = ch;
}

void Feeder::printPart(int from, int from_char, int to, int to_char)
{
	if(from < 0 || from >= m_lines.size())
		return;

	auto *ln = &m_lines.at(from);
	for(int i=0;i<from_char;i++)
		cerr << ' ';
	for(int i=from_char;i < ln->size();i++){
		cerr << ln->at(i);
	}
	cerr << endl;

	if(to > m_lines.size())
		return;

	for(int i=from+1;i<to;i++){
		cerr << m_lines.at(i) << endl;
	}

	if(to == m_lines.size())
		return;

	ln = &m_lines.at(to);
	for(int i=0;i<=to_char;i++){
		if(i < ln->size())
			cerr << ln->at(i);
	}
	cerr << endl;
}

void Feeder::printErrorPart(int from, int from_char, int to, int to_char)
{
	for(int i=from;i<=to;i++){
		if(i != from && i == to && to_char == 0)
			break;

		string left = "\tline" + to_string(i+1) + ": ";
		cerr << left <<  m_lines[i] << endl;
		if(i == from){
			cerr << "\t";
			for(int j=1-left.size();j<from_char;j++)
				cerr << " ";
			cerr << "^" << endl;
		}
	}
}

int Feeder::countIndent(void)
{
	if(atEnd())
		return 0;

	if(m_cur_char != 0)
		return -1;

	string *p = &m_lines[m_cur_line];
	int i = 0;
	while(i < (int)p->size()){
		if(strchr(" \t",p->at(i)) == NULL)
			break;

		i++;
	}

	return i;
}

bool Feeder::lineResidual(string *ans)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	if(ans != NULL)
		*ans = string(p->c_str()+m_cur_char,p->size()-m_cur_char);
	m_cur_char = 0;
	m_cur_line++;
	return true;
	
}
