#pragma once

#include <functional>


template <class T, class RType = void>
class DtorWrapper{
public:
	DtorWrapper(T x, std::function<RType()> f) : m_dtorfunc{ f } {};

	~DtorWrapper() { m_dtorfunc(); }
private:
	std::function<RType()> m_dtorfunc;
};

