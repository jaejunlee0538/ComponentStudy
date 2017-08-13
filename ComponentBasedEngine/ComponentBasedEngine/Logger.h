#pragma once
#include "Singleton.h"
#include <stdint.h>

namespace Logging {
	enum Channels {
		CHANNEL_GENERAL = 0,
		CHANNEL_CORE
	};

	enum VerbosityLevel {
		LEVEL_DEBUG=1,
		LEVEL_VERBOSE,
		LEVEL_INFO,
		LEVEL_WARN,
		LEVEL_ERROR,
		LEVEL_FATAL
	};

	const char* GetVerbosityString(VerbosityLevel verbosity);
	const char* GetChannelString(Channels channel);

	class ILogTarget {
	public:
		virtual ~ILogTarget(){}
		virtual void Log(const char* msg) = 0;
		virtual void Flush() = 0;
	};

	class Logger : public Singleton<Logger>
	{
		enum {
			MAX_NUM_TARGETS = 10,
			BUFFER_SIZE = 512
		};
	public:
		Logger();
		~Logger();

		bool AddLogTarget(ILogTarget* target);

		void EnableChannel(Channels channel, bool enable);
		bool IsChannelEnabled(Channels channel) const;

		void SetVerbosity(VerbosityLevel verbosity);
		VerbosityLevel GetVerbosity() const;

		void Flush();

		void Log(Channels channel, VerbosityLevel verbosity, const char* fmt, ...);
	private:
		int32_t		m_channelBits;
		VerbosityLevel	m_verbosity;

		size_t		m_numLogTargets;
		ILogTarget* m_logTargets[MAX_NUM_TARGETS];
	};
}

#define LOG_DEBUG(channel, fmt, ...)  Logging::Logger::GetSingleton().Log(channel, Logging::LEVEL_DEBUG, fmt, __VA_ARGS__)
#define LOG_VERBOSE(channel, fmt, ...)  Logging::Logger::GetSingleton().Log(channel, Logging::LEVEL_VERBOSE, fmt, __VA_ARGS__)
#define LOG_INFO(channel, fmt, ...)  Logging::Logger::GetSingleton().Log(channel, Logging::LEVEL_INFO, fmt, __VA_ARGS__)
#define LOG_WARN(channel, fmt, ...)  Logging::Logger::GetSingleton().Log(channel, Logging::LEVEL_WARN, fmt, __VA_ARGS__)
#define LOG_ERROR(channel, fmt, ...)  Logging::Logger::GetSingleton().Log(channel, Logging::LEVEL_ERROR, fmt, __VA_ARGS__)
#define LOG_FATAL(channel, fmt, ...)  Logging::Logger::GetSingleton().Log(channel, Logging::LEVEL_FATAL, fmt, __VA_ARGS__)
