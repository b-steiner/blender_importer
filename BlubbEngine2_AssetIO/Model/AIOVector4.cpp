#include "AIOVector4.hpp"

#include <math.h>
#include <algorithm>

using namespace AssetIO;

#pragma region Constructors & Destructor

AIOVector4::AIOVector4()
{
	values[0]=0;
	values[1]=0;
	values[2]=0;
	values[3]=1;
}

AIOVector4::AIOVector4(float* _values)
{
	for (int i = 0; i < 4; i++)
		values[i] = _values[i];
}

AIOVector4::AIOVector4(float _x, float _y, float _z)
{
	values[0]=_x;
	values[1]=_y;
	values[2]=_z;
	values[3]=1;
}

AIOVector4::AIOVector4(float _x, float _y, float _z, float _w)
{
	values[0]=_x;
	values[1]=_y;
	values[2]=_z;
	values[3]=_w;
}

AIOVector4::AIOVector4(const AIOVector4 &_refToVec)
{
	values[0]=_refToVec.values[0];
	values[1]=_refToVec.values[1];
	values[2]=_refToVec.values[2];
	values[3]=_refToVec.values[3];
}

AIOVector4::AIOVector4(const AIOVector3 &_refToVec, float _w)
{
	values[0]=_refToVec.X();
	values[1]=_refToVec.Y();
	values[2]=_refToVec.Z();
	values[3]=_w;
}

AIOVector4::~AIOVector4()
{
}

#pragma endregion

#pragma region Getter and Setter

float AIOVector4::X() const
{
	return values[0];
}

AIOVector4& AIOVector4::X(float _x)
{
	values[0]=_x;
	return *this;
}

float AIOVector4::Y() const
{
	return values[1];
}

AIOVector4& AIOVector4::Y(float _y)
{
	values[1]=_y;
	return *this;
}

float AIOVector4::Z() const
{
	return values[2];
}

AIOVector4& AIOVector4::Z(float _z)
{
	values[2]=_z;
	return *this;
}

float AIOVector4::W() const
{
	return values[3];
}

AIOVector4& AIOVector4::W(float _w)
{
	values[3]=_w;
	return *this;
}

float* AIOVector4::GetGlVector() const
{
	return (float*)(&(values[0]));
}

AIOVector3 AIOVector4::GetVec3() const
{
	return AIOVector3(X(), Y(), Z());
}

#pragma endregion

#pragma region Mathematical Operations

AIOVector4& AIOVector4::Homogenize()
{
	if(values[3]!=1.0f){
		values[0]/=values[3];
		values[1]/=values[3];
		values[2]/=values[3];
		values[3]=1;
	}
	return *this;
}

AIOVector4 AIOVector4::Homogenize(const AIOVector4& _vector)
{
	if(_vector.values[3] != 1.0f)
	{
		float rezi = 1.0f / _vector.values[3];
		return AIOVector4(_vector.values[0] * rezi, _vector.values[1] * rezi, _vector.values[2] * rezi, 1.0f);
	}
	
	return _vector;
}

AIOVector4& AIOVector4::Normalize3()
{
	float length=sqrt(values[0]*values[0] + values[1]*values[1] + values[2]*values[2]);
	values[0]/=length;
	values[1]/=length;
	values[2]/=length;
	return *this;
}

AIOVector4 AIOVector4::Normalize3(const AIOVector4 &_vector)
{
	return AIOVector4(_vector).Normalize3();
}

AIOVector4& AIOVector4::Normalize()
{
	float length=sqrt(values[0]*values[0] + values[1]*values[1] + values[2]*values[2] + values[3]*values[3]);
	values[0]/=length;
	values[1]/=length;
	values[2]/=length;
	values[3]/=length;
	return *this;
}

AIOVector4 AIOVector4::Normalize(const AIOVector4 &_vector)
{
	return AIOVector4(_vector).Normalize();
}
			
float AIOVector4::Length3() const
{
	return sqrt(values[0]*values[0] + values[1]*values[1] + values[2]*values[2]);
}

float AIOVector4::Length3(const AIOVector4 &_vector){
	return _vector.Length3();
}

float AIOVector4::Length() const
{
	return sqrt(values[0]*values[0] + values[1]*values[1] + values[2]*values[2] + values[3]*values[3]);
}

float AIOVector4::Length(const AIOVector4 &_vector)
{
	return _vector.Length();
}

float AIOVector4::Dot3(const AIOVector4 &_left, const AIOVector4 &_right)
{
	return _left.values[0]*_right.values[0] + 
		   _left.values[1]*_right.values[1] + 
		   _left.values[2]*_right.values[2];
}

float AIOVector4::Dot(const AIOVector4 &_left, const AIOVector4 &_right)
{
	return _left.values[0]*_right.values[0] + 
		   _left.values[1]*_right.values[1] + 
		   _left.values[2]*_right.values[2] +
		   _left.values[3]*_right.values[3];
}

AIOVector4 AIOVector4::Cross3(const AIOVector4 &_left, const AIOVector4 &_right)
{
	return AIOVector4(
		_left.Y() * _right.Z() - _left.Z() * _right.Y(),
		_left.Z() * _right.X() - _left.X() * _right.Z(),
		_left.X() * _right.Y() - _left.Y() * _right.X()
		);
}

AIOVector4 AIOVector4::ComponentMax(const AIOVector4& _v1, const AIOVector4& _v2)
{
	return AIOVector4(std::max(_v1.X(), _v2.X()), std::max(_v1.Y(), _v2.Y()), std::max(_v1.Z(), _v2.Z()), std::max(_v1.W(), _v2.W()));
}

AIOVector4 AIOVector4::ComponentMin(const AIOVector4& _v1, const AIOVector4& _v2)
{
	return AIOVector4(std::min(_v1.X(), _v2.X()), std::min(_v1.Y(), _v2.Y()), std::min(_v1.Z(), _v2.Z()), std::min(_v1.W(), _v2.W()));
}

AIOVector4 AIOVector4::Interpolate(const AIOVector4& _v1, const AIOVector4& _v2, float _a1, float _a2)
{
	float fac = _a1 / (_a1 + _a2);
	AIOVector4 diff = _v2 - _v1;

	return AIOVector4(_v1.X() + fac * diff.X(), _v1.Y() + fac * diff.Y(), _v1.Z() + fac * diff.Z(), _v1.W() + fac * diff.W());
}

#pragma endregion

#pragma region Operators

AIOVector4 AIOVector4::operator + (const AIOVector4 &_rhs) const
{
	return AIOVector4(values[0]+_rhs.values[0], values[1]+_rhs.values[1], values[2]+_rhs.values[2], values[3]+_rhs.values[3]);
}

AIOVector3 AIOVector4::operator + (const AIOVector3 &_rhs) const
{
	return AIOVector3(values[0]+_rhs.GetGlVector()[0], values[1]+_rhs.GetGlVector()[1], values[2]+_rhs.GetGlVector()[2]);
}
			
AIOVector4 AIOVector4::operator - (const AIOVector4 &_rhs) const
{
	return AIOVector4(values[0]-_rhs.values[0], values[1]-_rhs.values[1], values[2]-_rhs.values[2], values[3]-_rhs.values[3]);
}

AIOVector3 AIOVector4::operator - (const AIOVector3 &_rhs) const
{
	return AIOVector3(values[0]-_rhs.GetGlVector()[0], values[1]-_rhs.GetGlVector()[1], values[2]-_rhs.GetGlVector()[2]);
}

AIOVector4 AIOVector4::operator * (float _scalar) const		
{
	return AIOVector4(values[0]*_scalar, values[1]*_scalar, values[2]*_scalar, values[3]*_scalar);
}

AIOVector4 AIOVector4::operator / (float _scalar) const		
{
	float rezi = 1.0f / _scalar;
	return AIOVector4(values[0]*rezi, values[1]*rezi, values[2]*rezi, values[3]*rezi);
}

AIOVector4& AIOVector4::operator += (const AIOVector3 &_rhs)
{
	values[0]+=_rhs.GetGlVector()[0];
	values[1]+=_rhs.GetGlVector()[1];
	values[2]+=_rhs.GetGlVector()[2];
	return *this;
}

AIOVector4& AIOVector4::operator += (const AIOVector4 &_rhs)
{
	values[0]+=_rhs.values[0];
	values[1]+=_rhs.values[1];
	values[2]+=_rhs.values[2];
	values[3]+=_rhs.values[3];
	return *this;
}

AIOVector4& AIOVector4::operator -= (const AIOVector3 &_rhs)
{
	values[0]-=_rhs.GetGlVector()[0];
	values[1]-=_rhs.GetGlVector()[1];
	values[2]-=_rhs.GetGlVector()[2];
	return *this;
}

AIOVector4& AIOVector4::operator -= (const AIOVector4 &_rhs)
{
	values[0]-=_rhs.GetGlVector()[0];
	values[1]-=_rhs.GetGlVector()[1];
	values[2]-=_rhs.GetGlVector()[2];
	values[3]-=_rhs.GetGlVector()[3];
	return *this;
}

AIOVector4& AIOVector4::operator *= (float _scalar)
{
	values[0]*=_scalar;
	values[1]*=_scalar;
	values[2]*=_scalar;
	values[3]*=_scalar;
	return *this;
}

AIOVector4& AIOVector4::operator /= (float _scalar)
{
	float rezi = 1.0f / _scalar;
	values[0]*=rezi;
	values[1]*=rezi;
	values[2]*=rezi;
	values[3]*=rezi;
	return *this;
}

bool AIOVector4::operator == (const AIOVector4 &_rhs) const
{
	if(values[0]!=_rhs.values[0]) return false;
	if(values[1]!=_rhs.values[1]) return false;
	if(values[2]!=_rhs.values[2]) return false;
	if(values[3]!=_rhs.values[3]) return false;
	return true;
}

bool AIOVector4::operator != (const AIOVector4 &_toCompare) const
{
	if (values[0] != _toCompare.values[0]) return true;
	if (values[1] != _toCompare.values[1]) return true;
	if (values[2] != _toCompare.values[2]) return true;
	if (values[3] != _toCompare.values[3]) return true;
	return false;
}

AIOVector4& AIOVector4::operator = (const AIOVector4 &_toCompare)
{
	values[0] = _toCompare.values[0];
	values[1] = _toCompare.values[1];
	values[2] = _toCompare.values[2];
	values[3] = _toCompare.values[3];
	return *this;
}

#pragma endregion

std::ostream& operator <<(std::ostream &os, const AIOVector4 &obj)
{
	os << "[" << obj.X() << ", " << obj.Y() << ", " << obj.Z() << ", " << obj.W() << "]";
	return os;
}