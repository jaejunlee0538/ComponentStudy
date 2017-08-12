#pragma once
#include "MessageTypes.h"
enum MessagePriority : int8_t{
	MSG_PRIORITY_TOP,
	MSG_PRIORITY_HIGH,
	MSG_PRIORITY_MIDDLE,
	MSG_PRIORITY_LOW
};
struct Variant {
	enum Types {
		VARIANT_TYPE_INTEGER32,
		VARIANT_TYPE_FLOAT32,
		VARIANT_TYPE_BOOL,
		VARIANT_TYPE_STRING_ID,

		VARIANT_NUM_TYPES
	};
	Types type;
	union {
		int32_t		m_Interger32;
		float		m_Float32;
		uint32_t	m_Uinteger32;
		bool		m_Bool;
	};
};
struct Message
{
	Message(MessageTypes type, MessagePriority priority = MSG_PRIORITY_MIDDLE)
		:m_msgType(type), m_priority(priority)
	{
	}

	virtual ~Message(){
	}
	
	MessageTypes		m_msgType;
	MessagePriority		m_priority;
	uint32_t			m_postTime;
};

