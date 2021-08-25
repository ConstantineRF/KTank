#pragma once


#include <cmath>
#include <algorithm>

template<typename T>
class Vec2_
{
public:
	Vec2_() = default;
	Vec2_(T x_in, T y_in)
		:
		x(x_in),
		y(y_in)
	{}
	template<typename S>
	explicit Vec2_(const Vec2_<S>& src)
		:
		x((T)src.x),
		y((T)src.y)
	{}
	Vec2_ operator+(const Vec2_& rhs) const
	{
		return Vec2_(x + rhs.x, y + rhs.y);
	}
	Vec2_& operator+=(const Vec2_& rhs)
	{
		return *this = *this + rhs;
	}
	Vec2_ operator*(T rhs) const
	{
		return Vec2_(x * rhs, y * rhs);
	}
	Vec2_& operator*=(T rhs)
	{
		return *this = *this * rhs;
	}
	Vec2_ operator-(const Vec2_& rhs) const
	{
		return Vec2_(x - rhs.x, y - rhs.y);
	}
	Vec2_& operator-=(const Vec2_& rhs)
	{
		return *this = *this - rhs;
	}
	T GetLength() const
	{
		return (T)std::sqrt(GetLengthSq());
	}
	T GetLengthSq() const
	{
		return x * x + y * y;
	}
	T LengthSq() const { return GetLengthSq(); }
	Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}
	Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if (len != (T)0)
		{
			return *this * ((T)1 / len);
		}
		return *this;
	}
	/*
	static const Vec2_ Vec2_::UnitX()
	{
		return Vec2_((T) 1.0f, (T) 0.0f);
	};
	
	static const Vec2_ Vec2_::UnitY()
	{
		return Vec2_((T) 0.0f, (T) 1.0f);
	};
	*/
	static const Vec2_ UnitX()
	{
		return Vec2_((T) 1.0f, (T) 0.0f);
	};
	
	static const Vec2_ UnitY()
	{
		return Vec2_((T) 0.0f, (T) 1.0f);
	};

	float L1Norm() const
	{
		return abs(x) + abs(y);
	}
	float LInfNorm() const
	{
		const T absx = abs(x), absy = abs(y);
		return absx > absy ? absx : absy;
	}
	Vec2_ GetRotated90() const
	{
		return Vec2_(-y, x);
	}
	bool IsWithinRectangle(const Vec2_ & a, const Vec2_ & b) const
	{
		T minx = std::min(a.x, b.x);
		T maxx = std::max(a.x, b.x);
		T miny = std::min(a.y, b.y);
		T maxy = std::max(a.y, b.y);
		return (x >= minx) && (x <= maxx) && (y >= miny) && (y <= maxy);
	}

public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2;
typedef Vec2_<int> Vei2;

