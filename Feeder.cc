#include "Feeder.h"
#include <fstream>
#include <iostream>
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
		if(c == ' ' || c == '#')
			break;

		if(! isAlphabet(c) && ! isNum(c) && c != '.'
			&& c != '_' && c != '-' && c != '/')
			return false;
	}
	if(i == m_cur_char)
		return false;

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i;
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
		char c = p->at(i);
		if( c < 'a' || c > 'z')
			break;
	}

	if(i == m_cur_char)
		return false;

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i;
	checkEol(p);
	return true;
}

bool Feeder::blank(string *ans)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	checkEol(p);
	p = &m_lines[m_cur_line];

	if(outOfRange() || p->size() < 1)
		return false;

	int i = m_cur_char;
 	if(p->at(i) != ' ' && p->at(i) != '\t')
		return false;

	for(;i < (int)p->length();i++){
		char c = p->at(i);
		if( c != ' ' && c != '\t')
			break;
	}

	if(ans != NULL)
		*ans = string(p->c_str()+m_cur_char,i-m_cur_char);

	m_cur_char = i;
	checkEol(p);
	return true;
}

bool Feeder::path(string *ans)
{
	return command(ans);
}

bool Feeder::pipe(string *ans)
{
	if(outOfRange())
		return false;

	bool exist = false;
	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;
	for( ;i < (int)p->length();i++){
		char c = p->at(i);
		if(c == '>'){
			if(i >= (int)p->length()-2)
				return false;
			if(p->at(i+1) == '>' && p->at(i+2) == '='){
				exist = true;
				i += 3;
				break;
			}
		}
	}
	if(! exist)
		return false;

	if(ans != NULL)
		*ans = string(p->c_str()+m_cur_char,i-m_cur_char);

	m_cur_char = i;
	checkEol(p);
	return true;
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

	blank(NULL);

	if(!positiveInt(pos)){
		// It must be a positive integer after '['
		// in this stage. (A variable should be permitted
		// in future)
		m_error_msg = "Invalid array pos";
		throw this;
	}

	blank(NULL);

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
		if(c == '>' || c == ' ' || c == '#' || c == '[')
			break;

		if(! isAlphabet(c) && ! isNum(c) && c != '_' && c != '-')
			return false;
	}

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i;
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

	if(p->at(i) != '-' && (p->at(i) < '0' || p->at(i) > '9'))
		return false;

	i++;

	for(;i < (int)p->length();i++){
		char c = p->at(i);
		if(c == ' ' || c == '\t')
			break;
	}

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i;
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

	if(p->at(i) != '\''){
		return false;
	}

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

bool Feeder::comment(string *ans)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];

	// a blank line is treated as a comment
	if(p->length() == 0){
		*ans = "";
		m_cur_char = 0;
		m_cur_line++;

		while(m_cur_line < (int)m_lines.size()){
			if(m_lines[m_cur_line].length() != 0)
				break;

			*ans += "\n";
			m_cur_line++;
		}
		return true;
	}

	// #... is a comment
	if(p->at(m_cur_char) != '#')
		return false;

	*ans = string(p->c_str()+m_cur_char,p->length()-m_cur_char);

	m_cur_char = 0;
	m_cur_line++;
	return true;
}

bool Feeder::positiveInt(long *pos)
{
	if(outOfRange())
		return false;

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;

	if(p->at(i) < '0' || p->at(i) > '9')
		return false;

	i++;
	for(;i < (int)p->length();i++){
		if(! isNum(p->at(i)) )
			break;
	}

	string str = string(p->c_str()+m_cur_char,i-m_cur_char);
	char *e = NULL;
	*pos = std::strtol(str.c_str(), &e, 10);

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

	bool ans = false;
	
	string *p = &m_lines[m_cur_line];
	if(p->substr(m_cur_char,s.length()) == s){
		ans = true;
		m_cur_char += s.length();
	}

	checkEol(p);
	return ans;
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
	while(p->at(i) == ' ' || p->at(i) == '\t')
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
			cerr << "^";
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
	while(i < p->size()){
		if(p->at(i) == ' ' || p->at(i) == '\t')
			i++;
		else
			break;
	}

	return i;
}

