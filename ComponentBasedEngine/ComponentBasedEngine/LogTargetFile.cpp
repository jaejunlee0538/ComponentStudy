#include "stdafx.h"
#include "LogTargetFile.h"

namespace Logging {

	LogTargetFile::LogTargetFile(const char* fileName)
	{
		m_file.open(fileName);
	}

	LogTargetFile::~LogTargetFile()
	{
	}

	void LogTargetFile::Log(const char * msg)
	{
		m_file << msg;
	}

	void LogTargetFile::Flush()
	{
		m_file.flush();
	}
}