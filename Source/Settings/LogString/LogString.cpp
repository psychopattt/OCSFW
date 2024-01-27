#include "LogString.h"

namespace MainSettings
{
	LogString Log = LogString();
}

LogString::LogString()
{
	log.reserve(1024);
}

void LogString::Clear()
{
	log.clear();
}

bool LogString::IsEmpty() const
{
	return log.empty();
}

LogString& LogString::operator <<(char* text)
{
	return *this << const_cast<const char*>(text);
}

LogString& LogString::operator <<(const char* text)
{
	log += text;
	return *this;
}

LogString& LogString::operator <<(const std::string& text)
{
	return *this << text.c_str();
}

LogString& LogString::operator <<(bool boolean)
{
	return *this << (boolean ? "true" : "false");
}

LogString::operator const char* () const
{
	return log.c_str();
}
