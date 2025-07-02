#pragma once

#include <string>


class MyException {
public:
	MyException(const std::string& error)
		: m_error{ error }
	{
		m_last_error = getLastError();
	}
	const std::string& getError() const { return m_error; }
	const std::string& getLastError() const { return m_error; }

private:
	std::string m_error;
	std::string m_last_error;
};