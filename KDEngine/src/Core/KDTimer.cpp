#include "KDTimer.h"

using namespace std::chrono;

namespace KDE
{
	KDTimer::KDTimer()
	{
		m_Last = steady_clock::now();
	}

	float KDTimer::Mark()
	{
		const auto old = m_Last;
		m_Last = steady_clock::now();
		const duration<float> frametime = m_Last - old;
		return frametime.count();
	}
	float KDTimer::Peek() const
	{
		return duration<float>(steady_clock::now() - m_Last).count();
	}
}