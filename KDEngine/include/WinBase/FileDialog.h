#pragma once
#include <string>
#include "WinBase/KDWindow.h"

namespace KDE
{
	class FileDialog
	{
	public:
		static std::string OpenFile(const char* filter, KDWindow& window);
		static std::string SaveFile(const char* filter, KDWindow& window);
	};
}