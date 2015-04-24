#define ENGINEBASE_EXPORT

#include "AIOMatrix4.hpp"
#include "AIOMatrix3.hpp"
#include <math.h>

using namespace AssetIO;

#pragma region Constructors & Destructor

AIOMatrix4::AIOMatrix4()
{
	for (int i = 0; i < 16; i++)
		values[i] = 0.0f;

	values[0] = 1.0f;
	values[5] = 1.0f;
	values[10] = 1.0f;
	values[15] = 1.0f;
}

AIOMatrix4::AIOMatrix4(float _diag)
{
	for (int i = 0; i < 16; i++)
		values[i] = 0.0f;

	values[0] = _diag;
	values[5] = _diag;
	values[10] = _diag;
	values[15] = _diag;
}

AIOMatrix4::AIOMatrix4(float _m11, float _m12, float _m13, float _m14, float _m21, float _m22, float _m23, float _m24, float _m31, float _m32, float _m33, float _m34, float _m41, float _m42, float _m43, float _m44)
{
	values[0] = _m11;
	values[1] = _m12;
	values[2] = _m13;
	values[3] = _m14;
	values[4] = _m21;
	values[5] = _m22;
	values[6] = _m23;
	values[7] = _m24;
	values[8] = _m31;
	values[9] = _m32;
	values[10] = _m33;
	values[11] = _m34;
	values[12] = _m41;
	values[13] = _m42;	
	values[14] = _m43;
	values[15] = _m44;
}

AIOMatrix4::AIOMatrix4(const float* _values)
{
	memcpy(values, _values, 16*sizeof(float));
}

AIOMatrix4::AIOMatrix4(const AIOMatrix4& _toCopy)
{
	memcpy(values, _toCopy.values, 16*sizeof(float));
}

AIOMatrix4::AIOMatrix4(AIOMatrix3& _toCopy)
{
	values[0] = _toCopy.GetGlMatrix()[0];
	values[1] = _toCopy.GetGlMatrix()[1];
	values[2] = _toCopy.GetGlMatrix()[2];
	values[3] = 0.0f;

	values[4] = _toCopy.GetGlMatrix()[3];
	values[5] = _toCopy.GetGlMatrix()[4];
	values[6] = _toCopy.GetGlMatrix()[5];
	values[7] = 0.0f;

	values[8] = _toCopy.GetGlMatrix()[6];
	values[9] = _toCopy.GetGlMatrix()[7];
	values[10] = _toCopy.GetGlMatrix()[8];
	values[11] = 0.0f;

	values[12] = 0.0f;
	values[13] = 0.0f;
	values[14] = 0.0f;
	values[15] = 1.0f;
}

AIOMatrix4::~AIOMatrix4() { }

#pragma endregion

#pragma region Getter and Setter

inline float AIOMatrix4::M11() const
{
	return values[0];
}
inline AIOMatrix4& AIOMatrix4::M11(float _value)
{
	values[0] = _value;
	return *this;
}
inline float AIOMatrix4::M12() const
{
	return values[1];
}
inline AIOMatrix4& AIOMatrix4::M12(float _value)
{
	values[1] = _value;
	return *this;
}
inline float AIOMatrix4::M13() const
{
	return values[2];
}
inline AIOMatrix4& AIOMatrix4::M13(float _value)
{
	values[2] = _value;
	return *this;
}
inline float AIOMatrix4::M14() const
{
	return values[3];
}
inline AIOMatrix4& AIOMatrix4::M14(float _value)
{
	values[3] = _value;
	return *this;
}

inline float AIOMatrix4::M21() const
{
	return values[4];
}
inline AIOMatrix4& AIOMatrix4::M21(float _value)
{
	values[4] = _value;
	return *this;
}
inline float AIOMatrix4::M22() const
{
	return values[5];
}
inline AIOMatrix4& AIOMatrix4::M22(float _value)
{
	values[5] = _value;
	return *this;
}
inline float AIOMatrix4::M23() const
{
	return values[6];
}
inline AIOMatrix4& AIOMatrix4::M23(float _value)
{
	values[6] = _value;
	return *this;
}
inline float AIOMatrix4::M24() const
{
	return values[7];
}
inline AIOMatrix4& AIOMatrix4::M24(float _value)
{
	values[7] = _value;
	return *this;
}

inline float AIOMatrix4::M31() const
{
	return values[8];
}
inline AIOMatrix4& AIOMatrix4::M31(float _value)
{
	values[8] = _value;
	return *this;
}
inline float AIOMatrix4::M32() const
{
	return values[9];
}
inline AIOMatrix4& AIOMatrix4::M32(float _value)
{
	values[9] = _value;
	return *this;
}
inline float AIOMatrix4::M33() const
{
	return values[10];
}
inline AIOMatrix4& AIOMatrix4::M33(float _value)
{
	values[10] = _value;
	return *this;
}
inline float AIOMatrix4::M34() const
{
	return values[11];
}
inline AIOMatrix4& AIOMatrix4::M34(float _value)
{
	values[11] = _value;
	return *this;
}

inline float AIOMatrix4::M41() const
{
	return values[12];
}
inline AIOMatrix4& AIOMatrix4::M41(float _value)
{
	values[12] = _value;
	return *this;
}
inline float AIOMatrix4::M42() const
{
	return values[13];
}
inline AIOMatrix4& AIOMatrix4::M42(float _value)
{
	values[13] = _value;
	return *this;
}
inline float AIOMatrix4::M43() const
{
	return values[14];
}
inline AIOMatrix4& AIOMatrix4::M43(float _value)
{
	values[14] = _value;
	return *this;
}
inline float AIOMatrix4::M44() const
{
	return values[15];
}
inline AIOMatrix4& AIOMatrix4::M44(float _value)
{
	values[15] = _value;
	return *this;
}

inline float* AIOMatrix4::GetGlMatrix() const
{
	return (float*)(&values[0]);
}

inline const bool AIOMatrix4::IsColumnMajor()
{
	return false;
}

inline AIOMatrix3 AIOMatrix4::GetMat3() const
{
	return AIOMatrix3(M11(), M12(), M13(), M21(), M22(), M23(), M31(), M32(), M33());
}

#pragma endregion

#pragma region Mathematical Operations

AIOMatrix4& AIOMatrix4::Transpose()
{
	float z = M12();
	M12(M21());
	M21(z);

	z = M13();
	M13(M31());
	M31(z);

	z = M14();
	M14(M41());
	M41(z);

	z = M23();
	M23(M32());
	M32(z);

	z = M24();
	M24(M42());
	M42(z);

	z = M34();
	M34(M43());
	M43(z);

	return *this;
}
AIOMatrix4 AIOMatrix4::Transpose(const AIOMatrix4& _matrix)
{
	return AIOMatrix4(_matrix.M11(), _matrix.M21(), _matrix.M31(), _matrix.M41(),
		_matrix.M12(), _matrix.M22(), _matrix.M32(), _matrix.M42(),
		_matrix.M13(), _matrix.M23(), _matrix.M33(), _matrix.M43(),
		_matrix.M14(), _matrix.M24(), _matrix.M34(), _matrix.M44());
}

AIOMatrix4& AIOMatrix4::Invert()
{
	AIOMatrix4 m = AIOMatrix4::Invert(*this);
	for (int i = 0; i < 16; i++)
		values[i] = m.values[i];
	return *this;
}
AIOMatrix4 AIOMatrix4::Invert(const AIOMatrix4& _matrix)
{
	return _matrix.Adj() / _matrix.Det();
}
float AIOMatrix4::Det() const
{
	return AIOMatrix3::Det(AIOMatrix3(values[1], values[5], values[9], values[2], values[6], values[10], values[3], values[7], values[11])) * -values[12] +
		AIOMatrix3::Det(AIOMatrix3(values[0], values[4], values[8], values[2], values[6], values[10], values[3], values[7], values[11])) * values[13] +
		AIOMatrix3::Det(AIOMatrix3(values[0], values[4], values[8], values[1], values[5], values[9], values[3], values[7], values[11])) * -values[14] +
		AIOMatrix3::Det(AIOMatrix3(values[0], values[4], values[8], values[1], values[5], values[9], values[2], values[6], values[10])) * values[15];
}
float AIOMatrix4::Det(const AIOMatrix4& _matrix)
{
	return _matrix.Det();
}
AIOMatrix4 AIOMatrix4::Adj() const
{
	return AIOMatrix4(
		M23()*M34()*M42() - M24()*M33()*M42() + M24()*M32()*M43() - M22()*M34()*M43() - M23()*M32()*M44() + M22()*M33()*M44(),
		M14()*M33()*M42() - M13()*M34()*M42() - M14()*M32()*M43() + M12()*M34()*M43() + M13()*M32()*M44() - M12()*M33()*M44(),
		M13()*M24()*M42() - M14()*M23()*M42() + M14()*M22()*M43() - M12()*M24()*M43() - M13()*M22()*M44() + M12()*M23()*M44(),
		M14()*M23()*M32() - M13()*M24()*M32() - M14()*M22()*M33() + M12()*M24()*M33() + M13()*M22()*M34() - M12()*M23()*M34(),
		M24()*M33()*M41() - M23()*M34()*M41() - M24()*M31()*M43() + M21()*M34()*M43() + M23()*M31()*M44() - M21()*M33()*M44(),
		M13()*M34()*M41() - M14()*M33()*M41() + M14()*M31()*M43() - M11()*M34()*M43() - M13()*M31()*M44() + M11()*M33()*M44(),
		M14()*M23()*M41() - M13()*M24()*M41() - M14()*M21()*M43() + M11()*M24()*M43() + M13()*M21()*M44() - M11()*M23()*M44(),
		M13()*M24()*M31() - M14()*M23()*M31() + M14()*M21()*M33() - M11()*M24()*M33() - M13()*M21()*M34() + M11()*M23()*M34(),
		M22()*M34()*M41() - M24()*M32()*M41() + M24()*M31()*M42() - M21()*M34()*M42() - M22()*M31()*M44() + M21()*M32()*M44(),
		M14()*M32()*M41() - M12()*M34()*M41() - M14()*M31()*M42() + M11()*M34()*M42() + M12()*M31()*M44() - M11()*M32()*M44(),
		M12()*M24()*M41() - M14()*M22()*M41() + M14()*M21()*M42() - M11()*M24()*M42() - M12()*M21()*M44() + M11()*M22()*M44(),
		M14()*M22()*M31() - M12()*M24()*M31() - M14()*M21()*M32() + M11()*M24()*M32() + M12()*M21()*M34() - M11()*M22()*M34(),
		M23()*M32()*M41() - M22()*M33()*M41() - M23()*M31()*M42() + M21()*M33()*M42() + M22()*M31()*M43() - M21()*M32()*M43(),
		M12()*M33()*M41() - M13()*M32()*M41() + M13()*M31()*M42() - M11()*M33()*M42() - M12()*M31()*M43() + M11()*M32()*M43(),
		M13()*M22()*M41() - M12()*M23()*M41() - M13()*M21()*M42() + M11()*M23()*M42() + M12()*M21()*M43() - M11()*M22()*M43(),
		M12()*M23()*M31() - M13()*M22()*M31() + M13()*M21()*M32() - M11()*M23()*M32() - M12()*M21()*M33() + M11()*M22()*M33()
		);
}
AIOMatrix4 AIOMatrix4::Adj(const AIOMatrix4& _matrix)
{
	return _matrix.Adj();
}

AIOMatrix4 AIOMatrix4::GetRotation(const AIOMatrix4& _matrix)
{
	AIOVector3 col1(_matrix.values[0], _matrix.values[4], _matrix.values[8]);
	AIOVector3 col2(_matrix.values[1], _matrix.values[5], _matrix.values[9]);
	AIOVector3 col3(_matrix.values[2], _matrix.values[6], _matrix.values[10]);

	col1.Normalize();
	col2.Normalize();
	col3.Normalize();

	return AIOMatrix4(col1.X(), col2.X(), col3.X(), 0.0f, col1.Y(), col2.Y(), col3.Y(), 0.0f, col1.Z(), col2.Z(), col3.Z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0);
}

AIOMatrix4 AIOMatrix4::RemoveScaling(const AIOMatrix4& _matrix)
{
	AIOVector3 col1(_matrix.values[0], _matrix.values[4], _matrix.values[8]);
	AIOVector3 col2(_matrix.values[1], _matrix.values[5], _matrix.values[9]);
	AIOVector3 col3(_matrix.values[2], _matrix.values[6], _matrix.values[10]);

	col1.Normalize();
	col2.Normalize();
	col3.Normalize();

	return AIOMatrix4(col1.X(), col2.X(), col3.X(), _matrix.M14(), col1.Y(), col2.Y(), col3.Y(), _matrix.M24(), col1.Z(), col2.Z(), col3.Z(), _matrix.M34(), 0.0f, 0.0f, 0.0f, 1.0);



	/*//Remove scaling
	__m128 row1 = _mm_load_ps(mat.values);
	__m128 row2 = _mm_load_ps(mat.values + 4);
	__m128 row3 = _mm_load_ps(mat.values + 8);

	__m128 row1s = _mm_mul_ps(row1, row1);
	__m128 row2s = _mm_mul_ps(row2, row2);
	__m128 row3s = _mm_mul_ps(row3, row3);

	row1s = _mm_add_ps(_mm_add_ps(row1s, row2s), row3s); //dot product over columns
	row2s = _mm_sqrt_ps(row1s); //Scalings

	row1 = _mm_div_ps(row1, row2s);
	row2 = _mm_div_ps(row2, row2s);
	row3 = _mm_div_ps(row3, row2s);

	_mm_store_ps(mat.values, row1);
	_mm_store_ps(mat.values + 4, row2);
	_mm_store_ps(mat.values + 8, row3);

	//Remote translation
	mat.M14(_matrix.M14());
	mat.M24(_matrix.M24());
	mat.M34(_matrix.M34());

	return mat;*/
}

AIOMatrix4 AIOMatrix4::RotateX(float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);
	return AIOMatrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosa, -sina, 0.0f, 0.0f, sina, cosa, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix4& AIOMatrix4::RotateX(AIOMatrix4& _matrix, float _angle) {

	float cosa = cos(_angle);
	float sina = sin(_angle);

	float z1 = _matrix.M12();
	_matrix.M12(z1 * cosa + _matrix.M13() * sina);
	_matrix.M13(_matrix.M13() * cosa - z1 * sina);

	z1 = _matrix.M22();
	_matrix.M22(z1 * cosa + _matrix.M23() * sina);
	_matrix.M23(_matrix.M23() * cosa - z1 * sina);

	z1 = _matrix.M32();
	_matrix.M32(z1 * cosa + _matrix.M33() * sina);
	_matrix.M33(_matrix.M33() * cosa - z1 * sina);

	z1 = _matrix.M42();
	_matrix.M42(z1 * cosa + _matrix.M43() * sina);
	_matrix.M43(_matrix.M43() * cosa - z1 * sina);

	return _matrix;
}
AIOMatrix4 AIOMatrix4::RotateY(float _angle)
{
	float sina = sin(_angle);
	float cosa = cos(_angle);
	return AIOMatrix4(cosa, 0.0f, sina, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sina, 0.0f, cosa, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix4& AIOMatrix4::RotateY(AIOMatrix4& _matrix, float _angle)
{
	float cosa = cos(_angle);
	float sina = sin(_angle);

	float z1 = _matrix.M11();
	_matrix.M11(z1 * cosa - _matrix.M13() * sina);
	_matrix.M13(_matrix.M13() * cosa + z1 * sina);

	z1 = _matrix.M21();
	_matrix.M21(z1 * cosa - _matrix.M23() * sina);
	_matrix.M23(_matrix.M23() * cosa + z1 * sina);

	z1 = _matrix.M31();
	_matrix.M31(z1 * cosa - _matrix.M33() * sina);
	_matrix.M33(_matrix.M33() * cosa + z1 * sina);

	z1 = _matrix.M41();
	_matrix.M41(z1 * cosa - _matrix.M43() * sina);
	_matrix.M43(_matrix.M43() * cosa + z1 * sina);

	return _matrix;
}
AIOMatrix4 AIOMatrix4::RotateZ(float _angle)
{
	float cosa = cos(_angle);
	float sina = sin(_angle);

	return AIOMatrix4(cosa, -sina, 0.0f, 0.0f, sina, cosa, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix4& AIOMatrix4::RotateZ(AIOMatrix4& _matrix, float _angle)
{ 
	float cosa = cos(_angle);
	float sina = sin(_angle);

	float z1 = _matrix.M11();
	_matrix.M11(z1 * cosa + _matrix.M12() * sina);
	_matrix.M12(_matrix.M12() * cosa - z1 * sina);

	z1 = _matrix.M21();
	_matrix.M21(z1 * cosa + _matrix.M22() * sina);
	_matrix.M22(_matrix.M22() * cosa - z1 * sina);

	z1 = _matrix.M31();
	_matrix.M31(z1 * cosa + _matrix.M32() * sina);
	_matrix.M32(_matrix.M32() * cosa - z1 * sina);

	z1 = _matrix.M41();
	_matrix.M41(z1 * cosa + _matrix.M42() * sina);
	_matrix.M41(_matrix.M42() * cosa - z1 * sina);

	return _matrix;
}
AIOMatrix4 AIOMatrix4::RotateAxis(const AIOVector3& _axis, float _angle)
{
	float cosa = cos(_angle);
	float sina = sin(_angle);
	float icos = 1.0f - cosa;

	return AIOMatrix4(cosa + _axis.X() * _axis.X() * icos,
		_axis.X() * _axis.Y() * icos - _axis.Z() * sina,
		_axis.X() * _axis.Z() * icos + _axis.Y() * sina,
		0.0f,
		_axis.X() * _axis.Y() * icos + _axis.Z() * sina,
		cosa + _axis.Y() * _axis.Y() * icos,
		_axis.Y() * _axis.Z() * icos - _axis.X() * sina,
		0.0f,
		_axis.X() * _axis.Z() * icos - _axis.Y() * sina,
		_axis.Y() * _axis.Z() * icos + _axis.X() * sina,
		cosa + _axis.Z() * _axis.Z() * icos,
		0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix4& AIOMatrix4::RotateAxis(AIOMatrix4& _matrix, const AIOVector3& _axis, float _angle)
{	
	_matrix *= AIOMatrix4::RotateAxis(_axis, _angle);
	return _matrix;
}
AIOMatrix4 AIOMatrix4::Translate(const AIOVector3& _direction)
{
	return AIOMatrix4(1.0f, 0.0f, 0.0f, _direction.X(), 0.0f, 1.0f, 0.0f, _direction.Y(), 0.0f, 0.0f, 1.0f, _direction.Z(), 0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix4& AIOMatrix4::Translate(AIOMatrix4& _matrix, const AIOVector3& _direction)
{
	_matrix.M14(_matrix.M14() + _matrix.M11() * _direction.X() + _matrix.M12() * _direction.Y() + _matrix.M13() * _direction.Z());
	_matrix.M24(_matrix.M24() + _matrix.M21() * _direction.X() + _matrix.M22() * _direction.Y() + _matrix.M23() * _direction.Z());
	_matrix.M34(_matrix.M34() + _matrix.M31() * _direction.X() + _matrix.M32() * _direction.Y() + _matrix.M33() * _direction.Z());
	_matrix.M44(_matrix.M44() + _matrix.M41() * _direction.X() + _matrix.M42() * _direction.Y() + _matrix.M43() * _direction.Z());

	return _matrix;
}
AIOMatrix4 AIOMatrix4::Scale(const AIOVector3& _scales)
{
	return AIOMatrix4(_scales.X(), 0.0f, 0.0f, 0.0f, 0.0f, _scales.Y(), 0.0f, 0.0f, 0.0f, 0.0f, _scales.Z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
AIOMatrix4& AIOMatrix4::Scale(AIOMatrix4& _matrix, const AIOVector3& _scale)
{
	_matrix.M11(_matrix.M11() * _scale.X());
	_matrix.M21(_matrix.M21() * _scale.X());
	_matrix.M31(_matrix.M31() * _scale.X());
	_matrix.M41(_matrix.M41() * _scale.X());

	_matrix.M12(_matrix.M12() * _scale.Y());
	_matrix.M22(_matrix.M22() * _scale.Y());
	_matrix.M32(_matrix.M32() * _scale.Y());
	_matrix.M42(_matrix.M42() * _scale.Y());

	_matrix.M13(_matrix.M13() * _scale.Z());
	_matrix.M23(_matrix.M23() * _scale.Z());
	_matrix.M33(_matrix.M33() * _scale.Z());
	_matrix.M43(_matrix.M43() * _scale.Z());

	return _matrix;
}
AIOMatrix4 AIOMatrix4::Persp(float _fov, float _aspectRatio, float _nearPlane, float _farPlane)
{
	float top = tan(_fov * 0.5f) * _nearPlane;
	float bottom = -top;
	float left = _aspectRatio * bottom;
	float right = _aspectRatio * top;

	return AIOMatrix4(
		(2.0f * _nearPlane) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
		0.0f, (2.0f * _nearPlane) / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
		0.0f, 0.0f, - (_farPlane + _nearPlane) / (_farPlane - _nearPlane), -2.0f * _farPlane * _nearPlane / (_farPlane - _nearPlane),
		0.0f, 0.0f, -1.0f, 0.0f);

	/*float h = 1.0f / tan(_fov * 0.5f);
	float negDepth = _nearPlane - _farPlane;

	return AIOMatrix4(
		h / _aspectRatio, 0.0f, 0.0f, 0.0f,
		0.0f, h, 0.0f, 0.0f,
		0.0f, 0.0f, _farPlane / negDepth, 2.0f * (_nearPlane * _farPlane) / negDepth,
		0.0f, 0.0f, -1.0f, 0.0f
		);*/
}
AIOMatrix4 AIOMatrix4::Ortho(float _left, float _right, float _bottom, float _top, float _nearPlane, float _farPlane)
{
	return AIOMatrix4(
		2.0f / (_right - _left), 0.0f, 0.0f, -(_right + _left) / (_right - _left),
		0.0f, 2.0f / (_top - _bottom), 0.0f,  -(_top + _bottom) / (_top - _bottom),
		0.0f, 0.0f, -2.0f / (_farPlane - _nearPlane), -(_farPlane + _nearPlane) / (_farPlane - _nearPlane),
		0.0f, 0.0f, 0.0f, 1.0f);
}

#pragma endregion

#pragma region Operators

AIOMatrix4 AIOMatrix4::operator * (const AIOMatrix4& _matrix) const
{
	AIOMatrix4 tmp(
		M11() * _matrix.M11() + M12() * _matrix.M21() + M13() * _matrix.M31() + M14() * _matrix.M41(),
		M11() * _matrix.M12() + M12() * _matrix.M22() + M13() * _matrix.M32() + M14() * _matrix.M42(),
		M11() * _matrix.M13() + M12() * _matrix.M23() + M13() * _matrix.M33() + M14() * _matrix.M43(),
		M11() * _matrix.M14() + M12() * _matrix.M24() + M13() * _matrix.M34() + M14() * _matrix.M44(),

		M21() * _matrix.M11() + M22() * _matrix.M21() + M23() * _matrix.M31() + M24() * _matrix.M41(),
		M21() * _matrix.M12() + M22() * _matrix.M22() + M23() * _matrix.M32() + M24() * _matrix.M42(),
		M21() * _matrix.M13() + M22() * _matrix.M23() + M23() * _matrix.M33() + M24() * _matrix.M43(),
		M21() * _matrix.M14() + M22() * _matrix.M24() + M23() * _matrix.M34() + M24() * _matrix.M44(),

		M31() * _matrix.M11() + M32() * _matrix.M21() + M33() * _matrix.M31() + M34() * _matrix.M41(),
		M31() * _matrix.M12() + M32() * _matrix.M22() + M33() * _matrix.M32() + M34() * _matrix.M42(),
		M31() * _matrix.M13() + M32() * _matrix.M23() + M33() * _matrix.M33() + M34() * _matrix.M43(),
		M31() * _matrix.M14() + M32() * _matrix.M24() + M33() * _matrix.M34() + M34() * _matrix.M44(),

		M41() * _matrix.M11() + M42() * _matrix.M21() + M43() * _matrix.M31() + M44() * _matrix.M41(),
		M41() * _matrix.M12() + M42() * _matrix.M22() + M43() * _matrix.M32() + M44() * _matrix.M42(),
		M41() * _matrix.M13() + M42() * _matrix.M23() + M43() * _matrix.M33() + M44() * _matrix.M43(),
		M41() * _matrix.M14() + M42() * _matrix.M24() + M43() * _matrix.M34() + M44() * _matrix.M44()
		);

	return tmp;
}
AIOMatrix4 AIOMatrix4::operator * (const float _scalar) const
{
	AIOMatrix4 m(values);
	m *= _scalar;
	return m;
}
AIOVector4 AIOMatrix4::operator * (const AIOVector4& _vector) const
{
	return AIOVector4(
		M11() * _vector.X() + M12() * _vector.Y() + M13() * _vector.Z() + M14() * _vector.W(),
		M21() * _vector.X() + M22() * _vector.Y() + M23() * _vector.Z() + M24() * _vector.W(),
		M31() * _vector.X() + M32() * _vector.Y() + M33() * _vector.Z() + M34() * _vector.W(),
		M41() * _vector.X() + M42() * _vector.Y() + M43() * _vector.Z() + M44() * _vector.W()
		);
}
AIOVector3 AIOMatrix4::operator * (const AIOVector3& _vector) const
{
	return AIOVector3(
		M11() * _vector.X() + M12() * _vector.Y() + M13() * _vector.Z(),
		M21() * _vector.X() + M22() * _vector.Y() + M23() * _vector.Z(),
		M31() * _vector.X() + M32() * _vector.Y() + M33() * _vector.Z()
		);
}
AIOMatrix4 AIOMatrix4::operator / (const float _scalar) const
{
	float rezi = 1.0f / _scalar;
	AIOMatrix4 m(values);
	m *= rezi;
	return m;
}
AIOMatrix4& AIOMatrix4::operator *= (const AIOMatrix4& _matrix)
{
	AIOMatrix4 z = *this;

	M11(z.M11() * _matrix.M11() + z.M12() * _matrix.M21() + z.M13() * _matrix.M31() + z.M14() * _matrix.M41());
	M12(z.M11() * _matrix.M12() + z.M12() * _matrix.M22() + z.M13() * _matrix.M32() + z.M14() * _matrix.M42());
	M13(z.M11() * _matrix.M13() + z.M12() * _matrix.M23() + z.M13() * _matrix.M33() + z.M14() * _matrix.M43());
	M14(z.M11() * _matrix.M14() + z.M12() * _matrix.M24() + z.M13() * _matrix.M34() + z.M14() * _matrix.M44());

	M21(z.M21() * _matrix.M11() + z.M22() * _matrix.M21() + z.M23() * _matrix.M31() + z.M24() * _matrix.M41());
	M22(z.M21() * _matrix.M12() + z.M22() * _matrix.M22() + z.M23() * _matrix.M32() + z.M24() * _matrix.M42());
	M23(z.M21() * _matrix.M13() + z.M22() * _matrix.M23() + z.M23() * _matrix.M33() + z.M24() * _matrix.M43());
	M24(z.M21() * _matrix.M14() + z.M22() * _matrix.M24() + z.M23() * _matrix.M34() + z.M24() * _matrix.M44());

	M31(z.M31() * _matrix.M11() + z.M32() * _matrix.M21() + z.M33() * _matrix.M31() + z.M34() * _matrix.M41());
	M32(z.M31() * _matrix.M12() + z.M32() * _matrix.M22() + z.M33() * _matrix.M32() + z.M34() * _matrix.M42());
	M33(z.M31() * _matrix.M13() + z.M32() * _matrix.M23() + z.M33() * _matrix.M33() + z.M34() * _matrix.M43());
	M34(z.M31() * _matrix.M14() + z.M32() * _matrix.M24() + z.M33() * _matrix.M34() + z.M34() * _matrix.M44());

	M41(z.M41() * _matrix.M11() + z.M42() * _matrix.M21() + z.M43() * _matrix.M31() + z.M44() * _matrix.M41());
	M42(z.M41() * _matrix.M12() + z.M42() * _matrix.M22() + z.M43() * _matrix.M32() + z.M44() * _matrix.M42());
	M43(z.M41() * _matrix.M13() + z.M42() * _matrix.M23() + z.M43() * _matrix.M33() + z.M44() * _matrix.M43());
	M44(z.M41() * _matrix.M14() + z.M42() * _matrix.M24() + z.M43() * _matrix.M34() + z.M44() * _matrix.M44());

	return *this;
}
AIOMatrix4& AIOMatrix4::operator *= (const float _scalar)
{
	for (int i = 0; i < 16; i++)
		values[i] *= _scalar;
	
	return *this;
}
AIOMatrix4& AIOMatrix4::operator /= (const float _scalar)
{
	float rezi = 1.0f / _scalar;

	for (int i = 0; i < 16; i++)
		values[i] *= rezi;
	
	return *this;
}
AIOMatrix4& AIOMatrix4::operator = (const AIOMatrix4& _matrix)
{
	if (this != &_matrix)
	{
		for (int i = 0; i < 16; i++)
			values[i] = _matrix.values[i];
	}

	return *this;
}
bool AIOMatrix4::operator == (const AIOMatrix4& _matrix) const
{
	for (int i = 0; i < 16; i++)
		if (values[i] != _matrix.values[i])
			return false;

	return true;
}
bool AIOMatrix4::operator != (const AIOMatrix4& _matrix) const
{
	for (int i = 0; i < 16; i++)
		if (values[i] != _matrix.values[i])
			return true;

	return false;
}

#pragma endregion

inline float AIOMatrix4::M(int _column, int _row) const
{
	return values[_row * 4 + _column];
}
inline void AIOMatrix4::M(int _column, int _row, float _value)
{
	values[_row * 4 + _column] = _value;
}