#pragma once

#include <functional>


class DtorWrapper
{
public:
	template <typename T>
	DtorWrapper(T x, std::function<void()> f) : m_dtorfunc{ f } {};

	~DtorWrapper() { m_dtorfunc(); }
private:
	std::function<void()> m_dtorfunc;
};

