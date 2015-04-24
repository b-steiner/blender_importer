#include "AIOVector3.hpp"

#include <math.h>
#include <algorithm>
#include "AIOVector4.hpp"

using namespace AssetIO;

#pragma region Constructors & Destructor

AIOVector3::AIOVector3(void)
{
	values[0] = 0.0f;
	values[1] = 0.0f;
	values[2] = 0.0f;
}

AIOVector3::AIOVector3(float _x, float _y, float _z)
{
	values[0] = _x;
	values[1] = _y;
	values[2] = _z;
}

AIOVector3::AIOVector3(const AIOVector3 &_refToVec) 
{
	for (int i = 0; i < 3; i++)
	{
		values[i] = _refToVec.values[i];
	}
}

AIOVector3::AIOVector3(float* _values)
{
	for (int i = 0; i < 3; i++)
		values[i] = _values[i];
}

AIOVector3::AIOVector3(const AIOVector4 &_vec4)
{
	values[0] = _vec4.X();
	values[1] = _vec4.Y();
	values[2] = _vec4.Z();
}

AIOVector3::~AIOVector3(void)
{ }

#pragma endregion

#pragma region Getter and Setter

float AIOVector3::X(void) const
{
	return this->values[0];
}

AIOVector3& AIOVector3::X(float _x)
{
	this->values[0] = _x;
	return *this;
}

float AIOVector3::Y(void) const
{
	return this->values[1];
}

AIOVector3& AIOVector3::Y(float _y)
{
	this->values[1] = _y;
	return *this;
}

float AIOVector3::Z(void) const
{
	return this->values[2];
}

AIOVector3& AIOVector3::Z(float _z)
{
	this->values[2] = _z;
	return *this;
}

float* AIOVector3::GetGlVector() const
{ 
	return (float*)(&values[0]); 
}

AIOVector4 AIOVector3::GetVec4() const
{
	return AIOVector4(X(), Y(), Z());
}

#pragma endregion

#pragma region Mathematical Methods

AIOVector3& AIOVector3::Normalize() 
{ 
	float length = this->Length();

	X(X() / length);
	Y(Y() / length);
	Z(Z() / length);

	return *this; 
}

AIOVector3 AIOVector3::Normalize(const AIOVector3 &_vector)
{ 
	AIOVector3 tmp;
	float length = _vector.Length();
	tmp.X(_vector.X() / length);
	tmp.Y(_vector.Y() / length);
	tmp.Z(_vector.Z() / length);
	return tmp;
}

float AIOVector3::Length() const
{ 
	return sqrt(pow(X(),2) + pow(Y(),2) + pow(Z(),2)); 
}

float AIOVector3::Length(AIOVector3 &_vector) 
{ 
	return sqrt(pow(_vector.X(),2) + pow(_vector.Y(),2) + pow(_vector.Z(),2)); 
}

float AIOVector3::Dot(const AIOVector3 &_left, const AIOVector3 &_right)
{ 
	return (_left.X() * _right.X() + _left.Y() * _right.Y() + _left.Z() * _right.Z());
}

AIOVector3 AIOVector3::Cross(const AIOVector3 &_left, const AIOVector3 &_right)
{ 
	AIOVector3 tmp; 
	tmp.X(_left.Y() * _right.Z() - _right.Y() * _left.Z());
	tmp.Y(-(_left.X() * _right.Z() - _right.X() * _left.Z()));
	tmp.Z(_left.X() * _right.Y() - _right.X() * _left.Y());
	return tmp; 
}

AIOVector3 AIOVector3::ComponentMax(const AIOVector3& _v1, const AIOVector3& _v2)
{
	return AIOVector3(std::max(_v1.X(), _v2.X()), std::max(_v1.Y(), _v2.Y()), std::max(_v1.Z(), _v2.Z()));
}

AIOVector3 AIOVector3::ComponentMin(const AIOVector3& _v1, const AIOVector3& _v2)
{
	return AIOVector3(std::min(_v1.X(), _v2.X()), std::min(_v1.Y(), _v2.Y()), std::min(_v1.Z(), _v2.Z()));
}

AIOVector3 AIOVector3::Interpolate(const AIOVector3& _v1, const AIOVector3& _v2, float _a1, float _a2)
{
	float fac = _a1 / (_a1 + _a2);
	AIOVector3 diff = _v2 - _v1;

	return AIOVector3(_v1.X() + fac * diff.X(), _v1.Y() + fac * diff.Y(), _v1.Z() + fac * diff.Z());
}

#pragma endregion

#pragma region Operators

AIOVector3 AIOVector3::operator + (const AIOVector3 &_toAdd) const
{ 
	return AIOVector3(X() + _toAdd.X(), Y() + _toAdd.Y(), Z() + _toAdd.Z());
}

AIOVector3 AIOVector3::operator + (const AIOVector4 &_toAdd) const
{ 
	return AIOVector3(X() + _toAdd.X(), Y() + _toAdd.Y(), Z() + _toAdd.Z());
}

AIOVector3 AIOVector3::operator - (const AIOVector3 &_toSubtract) const
{ 
	AIOVector3 tmp; 
	tmp.X(X() - _toSubtract.X());
	tmp.Y(Y() - _toSubtract.Y());
	tmp.Z(Z() - _toSubtract.Z());
	return tmp; 
}

AIOVector3 AIOVector3::operator - (const AIOVector4 &_toSubtract) const
{ 
	AIOVector3 tmp; 
	tmp.X(X() - _toSubtract.X());
	tmp.Y(Y() - _toSubtract.Y());
	tmp.Z(Z() - _toSubtract.Z());
	return tmp; 
}

AIOVector3 AIOVector3::operator * (float _toMultiply) const
{ 
	AIOVector3 tmp; 
	tmp.X(X() * _toMultiply);
	tmp.Y(Y() * _toMultiply);
	tmp.Z(Z() * _toMultiply);
	return tmp; 
}

AIOVector3 AIOVector3::operator/ (float _scalar) const
{
	float rezi = 1.0f / _scalar;
	return (*this) * rezi;
}
			
AIOVector3& AIOVector3::operator += (const AIOVector3 &_toAdd) 
{ 
	X(X() + _toAdd.X());
	Y(Y() + _toAdd.Y());
	Z(Z() + _toAdd.Z());
	return *this; 
}

AIOVector3& AIOVector3::operator += (const AIOVector4 &_toAdd) 
{ 
	X(X() + _toAdd.X());
	Y(Y() + _toAdd.Y());
	Z(Z() + _toAdd.Z());
	return *this; 
}

AIOVector3& AIOVector3::operator -= (const AIOVector3 &_toSubtract) 
{ 
	X(X() - _toSubtract.X());
	Y(Y() - _toSubtract.Y());
	Z(Z() - _toSubtract.Z());
	return *this; 
}

AIOVector3& AIOVector3::operator -= (const AIOVector4 &_toSubtract) 
{ 
	X(X() - _toSubtract.X());
	Y(Y() - _toSubtract.Y());
	Z(Z() - _toSubtract.Z());
	return *this; 
}

AIOVector3& AIOVector3::operator *= (float _toMultiply)
{ 
	X(X() * _toMultiply);
	Y(Y() * _toMultiply);
	Z(Z() * _toMultiply);
	return *this; 
}

AIOVector3& AIOVector3::operator /= (float _scalar)
{
	(*this) *= (1.0f / _scalar);
	return *this;
}

bool AIOVector3::operator == (const AIOVector3 &_toCompare) const
{ 
	if ((X() == _toCompare.X()) && (Y() == _toCompare.Y()) && (Z() == _toCompare.Z()))
		return true;
	else
		return false; 
}

bool AIOVector3::operator != (const AIOVector3 &_toCompare) const
{
	if ((X() == _toCompare.X()) && (Y() == _toCompare.Y()) && (Z() == _toCompare.Z()))
		return false;
	else
		return true; 
}

AIOVector3& AIOVector3::operator = (const AIOVector3& _copy)
{
	if (this != &_copy)
	{
		values[0] = _copy.X();
		values[1] = _copy.Y();
		values[2] = _copy.Z();
	}

	return *this;
}

#pragma endregion
