#include "AIOMatrix3.hpp"
#include <math.h>

using namespace AssetIO;

#ifndef Det2x2
#define Det2x2(a,b,c,d) (a*d-b*c)
#define NegDet2x2(a,b,c,d) (b*c-a*d)
#endif

#pragma region Constructors & Destructor

AIOMatrix3::AIOMatrix3()
{
	values[0] = 1.0f;
	values[1] = 0.0f;
	values[2] = 0.0f;

	values[3] = 0.0f;
	values[4] = 1.0f;
	values[5] = 0.0f;
	
	values[6] = 0.0f;
	values[7] = 0.0f;
	values[8] = 1.0f;
}
AIOMatrix3::AIOMatrix3(float _m11, float _m12, float _m13, float _m21, float _m22, float _m23, float _m31, float _m32, float _m33)
{
	values[0] = _m11;
	values[1] = _m12;
	values[2] = _m13;

	values[3] = _m21;
	values[4] = _m22;
	values[5] = _m23;

	values[6] = _m31;
	values[7] = _m32;
	values[8] = _m33;
}
AIOMatrix3::AIOMatrix3(float _diag)
{
	values[0] = _diag;
	values[1] = 0.0f;
	values[2] = 0.0f;

	values[3] = 0.0f;
	values[4] = _diag;
	values[5] = 0.0f;
	
	values[6] = 0.0f;
	values[7] = 0.0f;
	values[8] = _diag;
}
AIOMatrix3::AIOMatrix3(const AIOMatrix3& _refToMat)
{
	memcpy(values, _refToMat.values, 9 * sizeof(float));
}
AIOMatrix3::AIOMatrix3(const float* _values)
{
	memcpy(values, _values, 9 * sizeof(float));
}
AIOMatrix3::~AIOMatrix3()
{
}

#pragma endregion

#pragma region Getter and Setter

inline float AIOMatrix3::M11() const
{
	return values[0];
}
inline AIOMatrix3& AIOMatrix3::M11(float _value)
{
	values[0] = _value;
	return *this;
}
inline float AIOMatrix3::M12() const
{
	return values[1];
}
inline AIOMatrix3& AIOMatrix3::M12(float _value)
{
	values[1] = _value;
	return *this;
}
inline float AIOMatrix3::M13() const
{
	return values[2];
}
inline AIOMatrix3& AIOMatrix3::M13(float _value)
{
	values[2] = _value;
	return *this;
}

inline float AIOMatrix3::M21() const
{
	return values[3];
}
inline AIOMatrix3& AIOMatrix3::M21(float _value)
{
	values[3] = _value;
	return *this;
}
inline float AIOMatrix3::M22() const
{
	return values[4];
}
inline AIOMatrix3& AIOMatrix3::M22(float _value)
{
	values[4] = _value;
	return *this;
}
inline float AIOMatrix3::M23() const
{
	return values[5];
}
inline AIOMatrix3& AIOMatrix3::M23(float _value)
{
	values[5] = _value;
	return *this;
}

inline float AIOMatrix3::M31() const
{
	return values[6];
}
inline AIOMatrix3& AIOMatrix3::M31(float _value)
{
	values[6] = _value;
	return *this;
}
inline float AIOMatrix3::M32() const
{
	return values[7];
}
inline AIOMatrix3& AIOMatrix3::M32(float _value)
{
	values[7] = _value;
	return *this;
}
inline float AIOMatrix3::M33() const
{
	return values[8];
}
inline AIOMatrix3& AIOMatrix3::M33(float _value)
{
	values[8] = _value;
	return *this;
}

inline float AIOMatrix3::M(int _column, int _row)
{
	return values[_row * 3 + _column];
}
inline void AIOMatrix3::M(int _column, int _row, float _value)
{
	values[_row * 3 + _column] = _value;
}

inline float* AIOMatrix3::GetGlMatrix() const
{
	return (float*)(&values[0]);
}

inline const bool AIOMatrix3::IsColumnMajor()
{
	return false;
}

#pragma endregion

#pragma region Mathematical Operations

AIOMatrix3& AIOMatrix3::Transpose()
{
	float v = values[1];
	values[1] = values[3];
	values[3] = v;

	v = values[2];
	values[2] = values[6];
	values[6] = v;

	v = values[5];
	values[5] = values[7];
	values[7] = v;

	return *this;
}
AIOMatrix3 AIOMatrix3::Transpose(const AIOMatrix3& _matrix)
{
	AIOMatrix3 temp(_matrix.M11(), _matrix.M21(), _matrix.M31(), _matrix.M12(), _matrix.M22(), _matrix.M32(), _matrix.M13(), _matrix.M23(), _matrix.M33());

	return temp;
}

AIOMatrix3 AIOMatrix3::RotateX(float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);
	return AIOMatrix3(1.0f, 0.0f, 0.0f, 0.0f, cosa, -sina, 0.0f, sina, cosa);
}
AIOMatrix3& AIOMatrix3::RotateX(AIOMatrix3& _matrix, float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);

	float mz;
	mz = _matrix.M12();
	_matrix.M12(_matrix.M12() * cosa + _matrix.M13() * sina);
	_matrix.M13(_matrix.M13() * cosa - mz * sina);
	mz = _matrix.M22();
	_matrix.M22(_matrix.M22() * cosa + _matrix.M23() * sina);
	_matrix.M23(_matrix.M23() * cosa - mz * sina);
	mz = _matrix.M32();
	_matrix.M32(_matrix.M32() * cosa + _matrix.M33() * sina);
	_matrix.M33(_matrix.M33() * cosa - mz * sina);
	return _matrix;
}
AIOMatrix3 AIOMatrix3::RotateY(float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);
	return AIOMatrix3(cosa, 0.0f, sina, 0.0f, 1.0f, 0.0f, -sina, 0.0f, cosa);
}
AIOMatrix3& AIOMatrix3::RotateY(AIOMatrix3& _matrix, float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);

	float mz = _matrix.M11();
	_matrix.M11(_matrix.M11() * cosa - _matrix.M13() * sina);
	_matrix.M13(_matrix.M13() * cosa + mz * sina);
	mz = _matrix.M21();
	_matrix.M21(_matrix.M21() * cosa - _matrix.M23() * sina);
	_matrix.M23(_matrix.M23() * cosa + mz * sina);
	mz = _matrix.M31();
	_matrix.M31(_matrix.M31() * cosa - _matrix.M33() * sina);
	_matrix.M33(_matrix.M33() * cosa + mz * sina);

	return _matrix;
}
AIOMatrix3 AIOMatrix3::RotateZ(float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);
	return AIOMatrix3(cosa, -sina, 0.0f, sina, cosa, 0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix3& AIOMatrix3::RotateZ(AIOMatrix3& _matrix, float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);

	float mz = _matrix.M11();
	_matrix.M11(_matrix.M11() * cosa + _matrix.M12() * sina);
	_matrix.M12(_matrix.M12() * cosa - mz * sina);
	mz = _matrix.M21();
	_matrix.M21(_matrix.M21() * cosa + _matrix.M22() * sina);
	_matrix.M22(_matrix.M22() * cosa - mz * sina);
	mz = _matrix.M31();
	_matrix.M31(_matrix.M31() * cosa + _matrix.M32() * sina);
	_matrix.M32(_matrix.M32() * cosa - mz * sina);

	return _matrix;
}
AIOMatrix3& AIOMatrix3::RotateAxis(AIOMatrix3& _matrix, const AIOVector3& _axis, float _angle)
{
	_matrix *= AIOMatrix3::RotateAxis(_axis, _angle);
	return _matrix;
}
AIOMatrix3 AIOMatrix3::RotateAxis(const AIOVector3& _axis, float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);
	float icos = 1.0f - cosa;
	return AIOMatrix3(cosa + _axis.X() * _axis.X() * icos,
		_axis.X() * _axis.Y() * icos - _axis.Z() * sina,
		_axis.X() * _axis.Z() * icos + _axis.Y() * sina,
		_axis.X() * _axis.Y() * icos + _axis.Z() * sina,
		cosa + _axis.Y() * _axis.Y() * icos,
		_axis.Y() * _axis.Z() * icos - _axis.X() * sina,
		_axis.X() * _axis.Z() * icos - _axis.Y() * sina,
		_axis.Y() * _axis.Z() * icos + _axis.X() * sina,
		cosa + _axis.Z() * _axis.Z() * icos);
}
AIOMatrix3 AIOMatrix3::Scale(const AIOVector3& _scales)
{
	return AIOMatrix3(_scales.X(), 0.0f, 0.0f, 0.0f, _scales.Y(), 0.0f, 0.0f, 0.0f, _scales.Z());
}
AIOMatrix3& AIOMatrix3::Scale(AIOMatrix3& _matrix, const AIOVector3& _scale)
{
	_matrix.M11(_matrix.M11() * _scale.X());
	_matrix.M21(_matrix.M21() * _scale.X());
	_matrix.M31(_matrix.M31() * _scale.X());

	_matrix.M12(_matrix.M12() * _scale.Y());
	_matrix.M22(_matrix.M22() * _scale.Y());
	_matrix.M32(_matrix.M32() * _scale.Y());

	_matrix.M13(_matrix.M13() * _scale.Z());
	_matrix.M23(_matrix.M23() * _scale.Z());
	_matrix.M33(_matrix.M33() * _scale.Z());

	return _matrix;
}

AIOMatrix3 AIOMatrix3::Rotate2D(float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);
	return AIOMatrix3(cosa, -sina, 0.0f, sina, cosa, 0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix3& AIOMatrix3::Rotate2D(AIOMatrix3& _matrix, float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);

	float mz = _matrix.M11();
	_matrix.M11(_matrix.M11() * cosa + _matrix.M12() * sina);
	_matrix.M12(_matrix.M12() * cosa - mz * sina);
	mz = _matrix.M21();
	_matrix.M21(_matrix.M21() * cosa + _matrix.M22() * sina);
	_matrix.M22(_matrix.M22() * cosa - mz * sina);
	mz = _matrix.M31();
	_matrix.M31(_matrix.M31() * cosa + _matrix.M32() * sina);
	_matrix.M32(_matrix.M32() * cosa - mz * sina);

	return _matrix;
}
AIOMatrix3 AIOMatrix3::Translate2D(const AIOVector2& _direction)
{
	return AIOMatrix3(1.0f, 0.0f, _direction.X(), 0.0f, 1.0f, _direction.Y(), 0.0f, 0.0f, 1.0f);
}
AIOMatrix3& AIOMatrix3::Translate2D(AIOMatrix3& _matrix, const AIOVector2& _direction)
{
	_matrix.M13(_matrix.M11() * _direction.X() + _matrix.M12() * _direction.Y() + _matrix.M13());
	_matrix.M23(_matrix.M21() * _direction.X() + _matrix.M22() * _direction.Y() + _matrix.M23());
	return _matrix;
}
AIOMatrix3 AIOMatrix3::Scale2D(const AIOVector2& _scales)
{
	return AIOMatrix3(_scales.X(), 0.0f, 0.0f, 0.0f, _scales.Y(), 0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix3& AIOMatrix3::Scale2D(AIOMatrix3& _matrix, const AIOVector2& _scales)
{
	_matrix.M11(_matrix.M11() * _scales.X());
	_matrix.M21(_matrix.M21() * _scales.X());
	_matrix.M31(_matrix.M31() * _scales.X());

	_matrix.M12(_matrix.M12() * _scales.Y());
	_matrix.M22(_matrix.M22() * _scales.Y());
	_matrix.M32(_matrix.M32() * _scales.Y());

	return _matrix;
}

AIOMatrix3& AIOMatrix3::Invert()
{
	AIOMatrix3 m = AIOMatrix3::Invert(*this);
	memcpy(values, m.values, 9*sizeof(float));
	return *this;
}
AIOMatrix3 AIOMatrix3::Invert(const AIOMatrix3& _matrix)
{
	return Adj(_matrix) / Det(_matrix);
}

float AIOMatrix3::Det() const
{
	return (values[0]*values[4]*values[8]+values[1]*values[5]*values[6]+values[2]*values[3]*values[7]-
			values[2]*values[4]*values[6]-values[1]*values[3]*values[8]-values[0]*values[5]*values[7]);
}
float AIOMatrix3::Det(const AIOMatrix3& _matrix)
{
	return _matrix.Det();
}

AIOMatrix3 AIOMatrix3::Adj() const
{
	return AIOMatrix3(Det2x2(values[4], values[5], values[7], values[8]), NegDet2x2(values[1], values[2], values[7], values[8]), Det2x2(values[1], values[2], values[4], values[5]),
				 NegDet2x2(values[3], values[5], values[6], values[8]), Det2x2(values[0], values[2], values[6], values[8]), NegDet2x2(values[0], values[2], values[3], values[5]),
				 Det2x2(values[3], values[4], values[6], values[7]), NegDet2x2(values[0], values[1], values[6], values[7]), Det2x2(values[0], values[1], values[3], values[4]));
}
AIOMatrix3 AIOMatrix3::Adj(const AIOMatrix3& _matrix)
{
	return _matrix.Adj();
}

#pragma endregion

#pragma region Operators

AIOMatrix3 AIOMatrix3::operator * (const AIOMatrix3& _toMultiply) const
{
	AIOMatrix3 tmp;
	
	tmp.M11(M11() * _toMultiply.M11() + M12() * _toMultiply.M21() + M13() * _toMultiply.M31());
	tmp.M12(M11() * _toMultiply.M12() + M12() * _toMultiply.M22() + M13() * _toMultiply.M32());
	tmp.M13(M11() * _toMultiply.M13() + M12() * _toMultiply.M23() + M13() * _toMultiply.M33());

	tmp.M21(M21() * _toMultiply.M11() + M22() * _toMultiply.M21() + M23() * _toMultiply.M31());
	tmp.M22(M21() * _toMultiply.M12() + M22() * _toMultiply.M22() + M23() * _toMultiply.M32());
	tmp.M23(M21() * _toMultiply.M13() + M22() * _toMultiply.M23() + M23() * _toMultiply.M33());

	tmp.M31(M31() * _toMultiply.M11() + M32() * _toMultiply.M21() + M33() * _toMultiply.M31());
	tmp.M32(M31() * _toMultiply.M12() + M32() * _toMultiply.M22() + M33() * _toMultiply.M32());
	tmp.M33(M31() * _toMultiply.M13() + M32() * _toMultiply.M23() + M33() * _toMultiply.M33());

	return tmp;
}
AIOVector3 AIOMatrix3::operator * (const AIOVector3& _toMultiply) const
{
	AIOVector3 tmp;
	
	tmp.X(_toMultiply.X() * M11() + _toMultiply.Y() * M12() + _toMultiply.Z() * M13());
	tmp.Y(_toMultiply.X() * M21() + _toMultiply.Y() * M22() + _toMultiply.Z() * M23());
	tmp.Z(_toMultiply.X() * M31() + _toMultiply.Y() * M32() + _toMultiply.Z() * M33());

	return tmp;
}
AIOMatrix3 AIOMatrix3::operator * (const float _scalar) const
{
	AIOMatrix3 m(values);
	m *= _scalar;
	return m;
}
AIOMatrix3 AIOMatrix3::operator / (const float _scalar) const
{
	float rezi = 1.0f / _scalar;
	return (*this) * rezi;
}
AIOMatrix3& AIOMatrix3::operator *= (const AIOMatrix3& _toMultiply)
{
	AIOMatrix3 z = *this;

	M11(z.M11() * _toMultiply.M11() + z.M12() * _toMultiply.M21() + z.M13() * _toMultiply.M31());
	M12(z.M11() * _toMultiply.M12() + z.M12() * _toMultiply.M22() + z.M13() * _toMultiply.M32());
	M13(z.M11() * _toMultiply.M13() + z.M12() * _toMultiply.M23() + z.M13() * _toMultiply.M33());

	M21(z.M21() * _toMultiply.M11() + z.M22() * _toMultiply.M21() + z.M23() * _toMultiply.M31());
	M22(z.M21() * _toMultiply.M12() + z.M22() * _toMultiply.M22() + z.M23() * _toMultiply.M32());
	M23(z.M21() * _toMultiply.M13() + z.M22() * _toMultiply.M23() + z.M23() * _toMultiply.M33());

	M31(z.M31() * _toMultiply.M11() + z.M32() * _toMultiply.M21() + z.M33() * _toMultiply.M31());
	M32(z.M31() * _toMultiply.M12() + z.M32() * _toMultiply.M22() + z.M33() * _toMultiply.M32());
	M33(z.M31() * _toMultiply.M13() + z.M32() * _toMultiply.M23() + z.M33() * _toMultiply.M33());

	return *this;
}
AIOMatrix3& AIOMatrix3::operator *= (const float _scalar)
{
	for (int i = 0; i < 9; i++)
		values[i] *= _scalar;

	return *this;
}
AIOMatrix3& AIOMatrix3::operator /= (const float _scalar)
{
	float rezi = 1.0f / _scalar;
	*this *= rezi;
	return *this;
}
bool AIOMatrix3::operator == (const AIOMatrix3& _toCompare) const
{
	if (values[0] != _toCompare.values[0]) return false;
	if (values[1] != _toCompare.values[1]) return false;
	if (values[2] != _toCompare.values[2]) return false;
	if (values[3] != _toCompare.values[3]) return false;
	if (values[4] != _toCompare.values[4]) return false;
	if (values[5] != _toCompare.values[5]) return false;
	if (values[6] != _toCompare.values[6]) return false;
	if (values[7] != _toCompare.values[7]) return false;
	if (values[8] != _toCompare.values[8]) return false;

	return true;
}
bool AIOMatrix3::operator!= (const AIOMatrix3& _toCompare) const
{
	if (values[0] != _toCompare.values[0]) return true;
	if (values[1] != _toCompare.values[1]) return true;
	if (values[2] != _toCompare.values[2]) return true;
	if (values[3] != _toCompare.values[3]) return true;
	if (values[4] != _toCompare.values[4]) return true;
	if (values[5] != _toCompare.values[5]) return true;
	if (values[6] != _toCompare.values[6]) return true;
	if (values[7] != _toCompare.values[7]) return true;
	if (values[8] != _toCompare.values[8]) return true;

	return false;
}
AIOMatrix3 AIOMatrix3::operator = (AIOMatrix3& _copy)
{
	if (this != &_copy)
	{
		memcpy(values, _copy.values, 9 * sizeof(float));
	}
	return *this;
}

#pragma endregion