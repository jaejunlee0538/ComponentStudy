#pragma once
#include "Singleton.h"
#include <stdint.h>

namespace Logging {
	enum Channels {
		CHANNEL_GENERAL = 0
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
