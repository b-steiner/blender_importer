#include "AIOVector2.hpp"
#include <algorithm>

using namespace AssetIO;

#pragma region Constructors & Destructor

AIOVector2::AIOVector2()
{
	values[0] = 0.0f;
	values[1] = 0.0f;
}

AIOVector2::AIOVector2(float _x, float _y)
{
	values[0] = _x;
	values[1] = _y;
}

AIOVector2::AIOVector2(float* _values)
{
	values[0] = _values[0];
	values[1] = _values[1];
}

AIOVector2::AIOVector2(const AIOVector2& _refToVec)
{	
	memcpy(&values, &(_refToVec.values), sizeof(float) * 2);
}

AIOVector2::~AIOVector2()
{
}

#pragma endregion

#pragma region Getter and Setter

float AIOVector2::X() const
{
	return this->values[0];
}

AIOVector2& AIOVector2::X(float _x)
{
	values[0] = _x;
	return *this;
}

float AIOVector2::Y() const
{
	return this->values[1];
}

AIOVector2& AIOVector2::Y(float _y)
{
	values[1] = _y;
	return *this;
}

float* AIOVector2::GetGlVector() const
{
	return (float*)(&values[0]);
}

#pragma endregion

#pragma region Mathematical Operations

AIOVector2& AIOVector2::Normalize()
{
	float l = this->Length();

	this->values[0] /= l;
	this->values[1] /= l;

	return *this;
}

AIOVector2 AIOVector2::Normalize(const AIOVector2 &_vector)
{
	float l = _vector.Length();

	return AIOVector2(_vector.X() / l, _vector.Y() / l);
}

float AIOVector2::Length() const
{
	return sqrt(X() * X() + Y() * Y());
}

float AIOVector2::Length(const AIOVector2 &_vector)
{
	return sqrt(_vector.X() * _vector.X() + _vector.Y() * _vector.Y());
}

AIOVector2 AIOVector2::ComponentMax(const AIOVector2& _v1, const AIOVector2& _v2)
{
	return AIOVector2(std::max(_v1.X(), _v2.X()), std::max(_v1.Y(), _v2.Y()));
}

AIOVector2 AIOVector2::ComponentMin(const AIOVector2& _v1, const AIOVector2& _v2)
{
	return AIOVector2(std::min(_v1.X(), _v2.X()), std::min(_v1.Y(), _v2.Y()));
}

AIOVector2 AIOVector2::Interpolate(const AIOVector2& _v1, const AIOVector2& _v2, float _a1, float _a2)
{
	float fac = _a1 / (_a1 + _a2);
	AIOVector2 diff = _v2 - _v1;

	return AIOVector2(_v1.X() + fac * diff.X(), _v1.Y() + fac * diff.Y());
}

#pragma endregion

#pragma region Operators

AIOVector2 AIOVector2::operator+(const AIOVector2& _toAdd) const
{
	return AIOVector2(X() + _toAdd.X(), Y() + _toAdd.Y());
}

AIOVector2& AIOVector2::operator+=(const AIOVector2& _toAdd)
{
	X(X() + _toAdd.X());
	Y(Y() + _toAdd.Y());
	return *this;
}

AIOVector2 AIOVector2::operator-(const AIOVector2& _toSubtract) const
{
	return AIOVector2(X() - _toSubtract.X(), Y() - _toSubtract.Y());
}

AIOVector2& AIOVector2::operator-=(const AIOVector2& _toSubtract)
{
	X(X() - _toSubtract.X());
	Y(Y() - _toSubtract.Y());
	return *this;
}

AIOVector2 AIOVector2::operator*(float _scalar) const
{
	return AIOVector2(X() * _scalar, Y() * _scalar);
}

AIOVector2& AIOVector2::operator*=(float _scalar)
{
	X(X() * _scalar);
	Y(Y() * _scalar);
	return *this;
}

AIOVector2 AIOVector2::operator/ (float _scalar) const
{
	float rezi = 1.0f / _scalar;
	return (*this) * rezi;
}

AIOVector2& AIOVector2::operator /= (float _scalar)
{
	(*this) *= (1.0f / _scalar);
	return *this;
}

bool AIOVector2::operator==(const AIOVector2& _toCompare) const
{
	if (X() == _toCompare.X() && Y() == _toCompare.Y())
		return true;
	else
		return false;
}

bool AIOVector2::operator!=(const AIOVector2& _toCompare) const
{
	if (X() == _toCompare.X() && Y() == _toCompare.Y())
		return false;
	else
		return true;
}

AIOVector2& AIOVector2::operator=(const AIOVector2& _copy)
{
	if (this != &_copy)
	{
		values[0] = _copy.X();
		values[1] = _copy.Y();
	}

	return *this;
}

#pragma endregion