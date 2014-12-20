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

	string *p = &m_lines[m_cur_line];
	int i = m_cur_char;
	for(;i < (int)p->length();i++){
		if(p->at(i) == ' '){
			break;
		}
	}
	*ans = string(p->c_str()+m_cur_char,i-m_cur_char);
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
