#include "TmpFile.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
using namespace std;

TmpFile::TmpFile(Feeder *f) : Element(f)
{
}

TmpFile::~TmpFile()
{

}

bool TmpFile::parse(void)
{
	string m_var_name;
	if(! m_feeder->tmpFile(&m_var_name))
		return false;

	string pid = to_string(getpid());
	m_file_name = "/tmp/" + pid + "-" + m_var_name;
	m_feeder->setVariable(&m_var_name,&m_file_name);
	m_feeder->setFileList(&m_file_name);
	return true;
}

int TmpFile::exec(void)
{
	return 0;
}

void TmpFile::printOriginalString(void)
{
	cerr << m_var_name;
}
