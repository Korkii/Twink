#pragma once

#include <string>


class MyException {
public:
	MyException(const std::string& error)
		: m_error{ error }
	{
	}
	const std::string& getError() const { return m_error; }
private:
	std::string m_error;
};