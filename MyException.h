#pragma once
#include <iostream>
#include <string>


class MyException
{
private:
	std::string m_error;
public:
	MyException(std::string error)
		: m_error{ error }
	{
	}
	const std::string& getError() const { return m_error; }
};