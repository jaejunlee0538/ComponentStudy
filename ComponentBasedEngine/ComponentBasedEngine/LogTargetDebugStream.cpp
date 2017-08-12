#include "stdafx.h"
#include "LogTargetDebugStream.h"
#include <Windows.h>
namespace Logging {
	LogTargetDebugStream::LogTargetDebugStream()
	{
	}

	LogTargetDebugStream::~LogTargetDebugStream()
	{
	}

	void LogTargetDebugStream::Log(const char * msg)
	{
		OutputDebugStringA(msg);
	}
}