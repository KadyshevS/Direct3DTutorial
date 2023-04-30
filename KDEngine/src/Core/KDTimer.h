#pragma once

#include <chrono>

namespace KDE
{
	class KDTimer
	{
	public:
		KDTimer();

		float Mark();
		float Peek() const;
	private:
		std::chrono::steady_clock::time_point m_Last;
	};
}