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

void Feeder::print(void)
{
	for(auto ln : m_lines){
		cout << ln << endl;
	}
}

void Feeder::close(void)
{
	if(m_ifs == NULL)
		return;

	m_ifs->close();
	m_ifs = NULL;
}

bool Feeder::getToken(string *ans)
{
	if(m_cur_line >= (int)m_lines.size())
		return false;

	bool comment = false;

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;
	for(;i < (int)p->length();i++){
		if(p->at(i) == ' ')
			break;
		if(p->at(i) == '#'){
			comment = true;
			break;
		}
	}

	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
	m_cur_char = i+1;
	if(comment)
		m_cur_char--;

	if(m_cur_char >= (int)p->length()){
		m_cur_char = 0;
		m_cur_line++;
	}
	return true;
}

bool Feeder::ungetToken(string *str)// false: should be stop
{
	if(m_cur_char == 0)
		m_cur_line--;

	if(m_cur_line < 0)
		return false;
		
	m_cur_char -= str->length() + 1;
	if(m_cur_char < 0)
		return false;

	return true;
}

// arg should be a string that is quoted by '.
// This function gives escaped characters as
// they are escaped.
bool Feeder::getArg(string *ans)
{
	if(m_cur_line >= (int)m_lines.size())
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
	if(m_cur_char >= (int)p->length()){
		m_cur_char = 0;
		m_cur_line++;
	}
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
	return (m_cur_line == m_lines.size()) && m_cur_char == 0;
}

bool Feeder::getComment(string *ans)
{
	if(m_cur_line >= (int)m_lines.size())
		return false;

	string *p = &m_lines[m_cur_line];

	// a blank line is considered as a comment
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
