// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <functional>
#include <map>
#include <list>
#include <vector>
#include <assert.h>
#include <memory>
#include <string>



#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#include "TimeManager.h"
#include "Logger.h"

#define SINGLETON(classname)	\
private:\
classname() {}\
~classname() {}\
classname(const classname&){}\
public:\
static classname* GetInstance(){static classname instance; return &instance;}
