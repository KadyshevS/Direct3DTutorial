#pragma once

#include <math.h>
#include <DirectXMath.h>

namespace KDE::Math
{
	constexpr float PI = 3.14159265f;
	constexpr double PI_D = 3.1415926535897932;

	template <typename T>
	constexpr auto Sq(const T& x)
	{
		return x * x;
	}

	template<typename T>
	T WrapAngle(T theta)
	{
		theta = fmod(theta, (T)360);
		if (theta < (T)0)
			theta += (T)360;
		return theta;
	}

	template<typename T>
	constexpr T Interpolate(const T& src, const T& dst, float alpha)
	{
		return src + (dst - src) * alpha;
	}

	template<typename T>
	constexpr T ToRadians(T deg)
	{
		return deg * PI / (T)180.0;
	}
}
