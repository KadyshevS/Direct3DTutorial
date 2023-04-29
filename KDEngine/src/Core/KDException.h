#pragma once
#include <exception>
#include <string>

namespace KDE
{
	class KDException : public std::exception
	{
	public:
		KDException(int line, const char* file);

		const char* what() const override;
		virtual const char* Type() const;
		int Line() const;
		const std::string& File() const;
		std::string StringOrigin() const;
	private:
		int m_Line;
		std::string m_File;
	protected:
		mutable std::string m_WhatBuffer;
	};
}