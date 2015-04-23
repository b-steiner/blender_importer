#pragma once

#include "../dllconfig.hpp"
#include "AIOMatrix3.hpp"
#include "AIOVector3.hpp"
#include "AIOVector4.hpp"

#include <iostream>

#define AIOMATRIX4_ALIGN_16 _declspec(align(16))

namespace AssetIO
{
	/// <summary>
	/// Defines a 4x4 matrix and provides functions for working with them
	/// </summary>
	class AIOMATRIX4_ALIGN_16 _AIO_DECLSPEC AIOMatrix4
	{
	public:
		#pragma region Constructors and Destructor

		/// <summary>
		/// Initializes a new AIOMatrix4 object and initializes it with a identity matrix
		/// </summary>
		AIOMatrix4();
		/// <summary>
		/// Initializes a new AIOMatrix4 object and sets the diagonal to a value
		/// </summary>
		/// <param name="_diag">Value of the diagonal elements</param>
		AIOMatrix4(float _diag);
		/// <summary>
		/// Initializes a new AIOMatrix4 object
		/// </summary>
		/// <param name="_m11">Value for the first column in the first row</param>
		/// <param name="_m12">Value for the second column in the first row</param>
		/// <param name="_m13">Value for the third column in the first row</param>
		/// <param name="_m14">Value for the fourth column in the first row</param>
		/// <param name="_m21">Value for the first column in the second row</param>
		/// <param name="_m22">Value for the second column in the second row</param>
		/// <param name="_m23">Value for the third column in the second row</param>
		/// <param name="_m24">Value for the fourth column in the second row</param>
		/// <param name="_m31">Value for the first column in the third row</param>
		/// <param name="_m32">Value for the second column in the third row</param>
		/// <param name="_m33">Value for the third column in the third row</param>
		/// <param name="_m34">Value for the fourth column in the third row</param>
		/// <param name="_m41">Value for the first column in the fourth row</param>
		/// <param name="_m42">Value for the second column in the fourth row</param>
		/// <param name="_m43">Value for the third column in the fourth row</param>
		/// <param name="_m44">Value for the fourth column in the fourth row</param>
		AIOMatrix4(float _m11, float _m12, float _m13, float _m14, float _m21, float _m22, float _m23, float _m24, float _m31, float _m32, float _m33, float _m34, float _m41, float _m42, float _m43, float _m44);
		/// <summary>
		/// Initializes a new AIOMatrix4 object
		/// </summary>
		/// <param name="_values">Values for the matrix</param>
		AIOMatrix4(const float* _values);
		/// <summary>
		/// Initializes a new AIOMatrix4 object
		/// </summary>
		/// <param name="_toCopy">Matrix to copy the values from</param>
		AIOMatrix4(const AIOMatrix4& _toCopy);
		/// <summary>
		/// Initializes a new AIOMatrix4 object
		/// </summary>
		/// <param name="_toCopy">Matrix to copy the values from</param>
		AIOMatrix4(AIOMatrix3& _toCopy);
		/// <summary>
		/// Deletes all the data of a AIOMatrix4 object
		/// </summary>
		~AIOMatrix4();

		#pragma endregion

		#pragma region Getter and Setter

		/// <summary>
		/// Returns the value of the first column in the first row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M11() const;
		/// <summary>
		/// Sets the value of the first column in the first row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M11(float _value);
		/// <summary>
		/// Returns the value of the second column in the first row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M12() const;
		/// <summary>
		/// Sets the value of the second column in the first row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M12(float _value);
		/// <summary>
		/// Returns the value of the third column in the first row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M13() const;
		/// <summary>
		/// Sets the value of the third column in the first row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M13(float _value);
		/// <summary>
		/// Returns the value of the forth column in the first row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M14() const;
		/// <summary>
		/// Sets the value of the fourth column in the first row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M14(float _value);

		/// <summary>
		/// Returns the value of the first column in the second row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M21() const;
		/// <summary>
		/// Sets the value of the first column in the second row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M21(float _value);
		/// <summary>
		/// Returns the value of the second column in the second row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M22() const;
		/// <summary>
		/// Sets the value of the second column in the second row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M22(float _value);
		/// <summary>
		/// Returns the value of the third column in the second row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M23() const;
		/// <summary>
		/// Sets the value of the third column in the second row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M23(float _value);
		/// <summary>
		/// Returns the value of the fourth column in the second row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M24() const;
		/// <summary>
		/// Sets the value of the fourth column in the second row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M24(float _value);

		/// <summary>
		/// Returns the value of the first column in the third row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M31() const;
		/// <summary>
		/// Sets the value of the first column in the third row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M31(float _value);
		/// <summary>
		/// Returns the value of the second column in the third row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M32() const;
		/// <summary>
		/// Sets the value of the second column in the third row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M32(float _value);
		/// <summary>
		/// Returns the value of the thrid column in the third row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M33() const;
		/// <summary>
		/// Sets the value of the third column in the third row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M33(float _value);
		/// <summary>
		/// Returns the value of the fourth column in the third row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M34() const;
		/// <summary>
		/// Sets the value of the fourth column in the third row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M34(float _value);

		/// <summary>
		/// Returns the value of the first column in the fourth row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M41() const;
		/// <summary>
		/// Sets the value of the first column in the fourth row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M41(float _value);
		/// <summary>
		/// Returns the value of the second column in the fourth row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M42() const;
		/// <summary>
		/// Sets the value of the second column in the fourth row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M42(float _value);
		/// <summary>
		/// Returns the value of the third column in the fourth row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M43() const;
		/// <summary>
		/// Sets the value of the third column in the fourth row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M43(float _value);
		/// <summary>
		/// Returns the value of the fourth column in the fourth row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M44() const;
		/// <summary>
		/// Sets the value of the fourth column in the fourth row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>A reference to the matrix</returns>
		AIOMatrix4& M44(float _value);

		/// <summary>
		/// Returns a pointer to an OpenGL array
		/// </summary>
		/// <returns>Pointer to OpenGL array</returns>
		float* GetGlMatrix() const;
		/// <summary>
		/// Returns if the matrix is stored in column order
		/// </summary>
		/// <returns>True if stored in column order, else when not</returns>
		static const bool IsColumnMajor();
		/// <summary>
		/// Returns the upper left 3x3 matrix
		/// </summary>
		/// <returns>The upper left 3x3 matrix</returns>
		AIOMatrix3 GetMat3() const;

		#pragma endregion

		#pragma region Mathematical Operations

		/// <summary>
		/// Transposes the matrix
		/// </summary>
		/// <returns>A reference to the transposed matrix</returns>
		AIOMatrix4& Transpose();
		/// <summary>
		/// Transposes a AIOMatrix4 object
		/// </summary>
		/// <param name="_matrix">Matrix to transpose</param>
		/// <returns>The transposed matrix</returns>
		static AIOMatrix4 Transpose(const AIOMatrix4& _matrix);

		/// <summary>
		/// Creates a rotation matrix around the x-axis
		/// </summary>
		/// <param name="_angle">The angle in radian to rotate</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix4 RotateX(float _angle);
		/// <summary>
		/// Rotates a matrix around the x-axis
		/// </summary>
		/// <param name="_matrix">The matrix to rotate</param>
		/// <param name="_angle">The angle in radian to rotate</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix4& RotateX(AIOMatrix4& _matrix, float _angle);
		/// <summary>
		/// Creates a rotation matrix around the y-axis
		/// </summary>
		/// <param name="_angle">The angle in radian to rotate</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix4 RotateY(float _angle);
		/// <summary>
		/// Rotates a matrix around the y-axis
		/// </summary>
		/// <param name="_matrix">The matrix to rotate</param>
		/// <param name="_angle">The angle in radian to rotate</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix4& RotateY(AIOMatrix4& _matrix, float _angle);
		/// <summary>
		/// Creates a rotation matrix around the y-axis
		/// </summary>
		/// <param name="_angle">The angle in radian to rotate</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix4 RotateZ(float _angle);
		/// <summary>
		/// Rotates a matrix around the z-axis
		/// </summary>
		/// <param name="_matrix">The matrix to rotate</param>
		/// <param name="_angle">The angle in radian to rotate</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix4& RotateZ(AIOMatrix4& _matrix, float _angle);
		/// <summary>
		/// Creates a rotation matrix around a defined matrix
		/// </summary>
		/// <param name="_axis">The axis to rotate around</param>
		/// <param name="_angle">The angle in radian to rotate</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix4 RotateAxis(const AIOVector3& _axis, float _angle);
		/// <summary>
		/// Rotates a matrix around a defined axis
		/// </summary>
		/// <param name="_matrix">The matrix to rotate</param>
		/// <param name="_axis">The axis to rotate around</param>
		/// <param name="_angle">The angle in radian to rotate</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix4& RotateAxis(AIOMatrix4& _matrix, const AIOVector3& _axis, float _angle);
		/// <summary>
		/// Creates a translation matrix
		/// </summary>
		/// <param name="_direction">The direction to translate</param>
		/// <returns>The translation matrix</returns>
		static AIOMatrix4 Translate(const AIOVector3& _direction);
		/// <summary>
		/// Translates a matrix
		/// </summary>
		/// <param name="_direction">The direction to translate</param>
		/// <param name="_matrix">The matrix to translate</param>
		/// <returns>A reference to the translated matrix</returns>
		static AIOMatrix4& Translate(AIOMatrix4& _matrix, const AIOVector3& _direction);
		/// <summary>
		/// Creates a scaling matrix
		/// </summary>
		/// <param name="_scales">The scaling factors</param>
		/// <returns>The scaling matrix</returns>
		static AIOMatrix4 Scale(const AIOVector3& _scales);
		/// <summary>
		/// Scales a matrix
		/// </summary>
		/// <param name="_matrix">The matrix to scale</param>
		/// <param name="_scale">The scaling values</param>
		/// <returns>A reference to the scaled matrix</returns>
		static AIOMatrix4& Scale(AIOMatrix4& _matrix, const AIOVector3& _scale);
		/// <summary>
		/// Creates a perspective projection matrix
		/// </summary>
		/// <param name="_fov">The opening angle of the camera in radian</param>
		/// <param name="_aspectRatio">The ration between width and height</param>
		/// <param name="_nearPlane">The distance to the near plane</param>
		/// <param name="_farPlane">The distance to the far plane</param>
		/// <returns>The projection matrix</returns>
		static AIOMatrix4 Persp(float _fov, float _aspectRatio, float _nearPlane, float _farPlane);
		/// <summary>
		/// Creates a orthographic projection matrix
		/// </summary>
		/// <param name="_left">Value of the left side</param>
		/// <param name="_right">Value of the right side</param>
		/// <param name="_bottom">The value of the bottom</param>
		/// <param name="_top">The value of the top</param>
		/// <param name="_nearPlane">The distance to the near plane</param>
		/// <param name="_farPlane">The distance to the far plane</param>
		/// <returns>The projection matrix</returns>
		static AIOMatrix4 Ortho(float _left, float _right, float _bottom, float _top, float _nearPlane, float _farPlane);

		/// <summary>
		/// Inverts the matrix
		/// </summary>
		/// <returns>A references to the inverted matrix</returns>
		AIOMatrix4& Invert();
		/// <summary>
		/// Inverts a matrix
		/// </summary>
		/// <param name="_matrix">Matrix to invert</param>
		/// <returns>The inverted matrix</returns>
		static AIOMatrix4 Invert(const AIOMatrix4& _matrix);
		/// <summary>
		/// Returns the determinant of the matrix
		/// </summary>
		/// <returns>The matrix' determinant</returns>
		float Det() const;
		/// <summary>
		/// Returns the determinant of a matrix
		/// </summary>
		/// <param name="_matrix">Matrix to calculate the determinant from</param>
		/// <returns>The matrix' determinant</returns>
		static float Det(const AIOMatrix4& _matrix);

		/// <summary>
		/// Returns only the rotational part of a matrix. Translations and scalings are removed.
		/// </summary>
		/// <param name="_matrix">Matrix to get the rotation from</param>
		/// <returns>Matrix containing only the rotation</returns>
		static AIOMatrix4 GetRotation(const AIOMatrix4& _matrix);
		/// <summary>
		/// Removes the scaling from a matrix
		/// </summary>
		/// <param name="_matrix">Matrix to remove the scaling from</param>
		/// <returns>Matrix containing only the rotation and translation</returns>
		static AIOMatrix4 RemoveScaling(const AIOMatrix4& _matrix);

		#pragma endregion

		#pragma region Operators

		/// <summary>
		/// Defines the multiplication of two matrices
		/// </summary>
		/// <param name="_matrix">The matrix to multiply with</param>
		/// <returns>The rotated matrix</returns>
		AIOMatrix4 operator * (const AIOMatrix4& _matrix) const;
		/// <summary>
		/// Pointwise multiplication of the matrix with a scalar value
		/// </summary>
		/// <param name="_scalar">The scalar value</param>
		/// <returns>The multiplied matrix</param>
		AIOMatrix4 operator * (const float _scalar) const;
		/// <summary>
		/// Defines the multiplication of a vector and a matrix
		/// </summary>
		/// <param name="_vector">The vector</param>
		/// <returns>The multiplied vector</returns>
		AIOVector4 operator * (const AIOVector4& _vector) const;
		/// <summary>
		/// Defines the multiplication of a vector and a matrix
		/// </summary>
		/// <param name="_vector">The vector</param>
		/// <returns>The multiplied vector</returns>
		AIOVector3 operator * (const AIOVector3& _vector) const;
		/// <summary>
		/// Pointwise division of the matrix with a scalar value
		/// </summary>
		/// <param name="_scalar">The scalar value</param>
		/// <returns>The divided matrix</param>
		AIOMatrix4 operator / (const float _scalar) const;

		/// <summary>
		/// Defines the multiplication of two matrices
		/// </summary>
		/// <param name="_matrix">To matrix to multiply with</param>
		/// <returns>A reference to the multiplied matrix</returns>
		AIOMatrix4& operator *= (const AIOMatrix4& _matrix);
		/// <summary>
		/// Pointwise multiplication of the matrix with a scalar value
		/// </summary>
		/// <param name="_scalar">The scalar value</param>
		/// <returns>A reference to the multiplied matrix</param>
		AIOMatrix4& operator *= (const float _scalar);
		/// <summary>
		/// Pointwise division of the matrix with a scalar value
		/// </summary>
		/// <param name="_scalar">The scalar value</param>
		/// <returns>A reference to the divided matrix</param>
		AIOMatrix4& operator /= (const float _scalar);
		/// <summary>
		/// Defines the assignment operator to get a deep copy
		/// </summary>
		/// <param name="_matrix">Source to copy from</param>
		/// <returns>Copied Mat3 object</returns>
		AIOMatrix4& operator = (const AIOMatrix4& _matrix);
		/// <summary>
		/// Defines the equal operator for two matrices
		/// </summary>
		/// <param name="_matrix">The matrix to compare</param>
		/// <returns>True if the two matrices are equal</returns>
		bool operator == (const AIOMatrix4& _matrix) const;
		/// <summary>
		/// Defines the unequal operator for two matrices
		/// </summary>
		/// <param name="_matrix">The matrix to compare</param>
		/// <returns>True if the two matrices are unequal</returns>
		bool operator != (const AIOMatrix4& _matrix) const;

		#pragma endregion

	private:
		float M(int _row, int _column) const;
		void M(int _row, int _column, float _value);

		/// <summary>
		/// Returns the adjunct matrix
		/// </summary>
		/// <returns>The adjunct matrix</returns>
		AIOMatrix4 Adj() const;
		/// <summary>
		/// Calculates the adjunct matrix
		/// </summary>
		/// <param name="_matrix">Matrix to calculate the adjunct matrix from</param>
		/// <returns>The adjunct matrix</returns>
		static AIOMatrix4 Adj(const AIOMatrix4& _matrix);

		float values[16];
	};
}