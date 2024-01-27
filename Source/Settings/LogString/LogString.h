#pragma once

#include <string>

class LogString
{
	public:
		LogString();
		void Clear();
		bool IsEmpty() const;

		LogString& operator <<(char* text);
		LogString& operator <<(const char* text);
		LogString& operator <<(const std::string& text);
		LogString& operator <<(bool boolean);

		template <typename Number>
		LogString& operator <<(Number number)
		{
			return *this << std::to_string(number).c_str();
		}

		operator const char* () const;

	private:
		std::string log;
};
