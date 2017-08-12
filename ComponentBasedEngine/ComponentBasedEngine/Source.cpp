// Game3DFramework.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GameApp.h"
#include "Singleton.h"
#include "LogTargetDebugStream.h"
#include "LogTargetFile.h"
#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
   
	TimeManager timeManager;
	Logging::Logger logger;
	Logging::LogTargetDebugStream logDebugStream;
	Logging::LogTargetFile	logFile("gameLog.log");
	logger.AddLogTarget(&logDebugStream);
	logger.AddLogTarget(&logFile);

	logger.Log(Logging::CHANNEL_GENERAL, Logging::LEVEL_INFO, "Creating GameApp(%d)", 100);
	GameApp app(hInstance);
	if (app.Init() == false) {
		return 0;
	}
	return app.Loop();
}
