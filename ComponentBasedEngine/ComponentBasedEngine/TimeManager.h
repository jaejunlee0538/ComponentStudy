#pragma once
#include "Singleton.h"
class TimeManager : public Singleton<TimeManager>
{
public:
	TimeManager();
	~TimeManager();

	void SetDesiredFPS(int fps);
	void SetFixedDeltatime(double deltaTime);
	void SetGameTimeScale(double scale);
	
	void Reset();
	void Update();

	double DeltaTime() const {
		return m_deltaGameTimeScaled;
	}
	double CurrentTime() const {
		return m_currentTimeGame;
	}
	double DeltaRealTime() const {
		return m_deltaTimeReal; 
	}
	double CurrentRealTime() const {
		return m_currentTimeReal;
	}
	uint32_t FramesCount() const {
		return m_framesCount; 
	}
private:
	void GetCurrentTimeMS(double& currentTime);
private:
	double m_deltaTimeGameOriginal;
	double m_deltaGameTimeScaled;
	double m_currentTimeGame;
	double m_deltaTimeReal;
	double m_currentTimeReal;
	double m_gameTimeScale;

	uint32_t m_framesCount;

	bool m_highResolution;
	__int64	m_tickBegin;
	double m_tickToSec;

	int	m_desiredFPS;
};

