#include "KDException.h"

#include <sstream>

namespace KDE
{
	KDException::KDException(int line, const char* file)
		: m_Line(line), m_File(file)
	{}

	const char* KDException::what() const
	{
		std::ostringstream oss;
		oss << Type() << std::endl
			<< StringOrigin();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* KDException::Type() const
	{
		return "KDEngine Exception";
	}
	int KDException::Line() const
	{
		return m_Line;
	}
	const std::string& KDException::File() const
	{
		return m_File;
	}
	std::string KDException::StringOrigin() const
	{
		std::ostringstream oss;
		oss << "[File] " << m_File << '\n'
			<< "[Line] " << m_Line;
		
		return oss.str();
	}
}