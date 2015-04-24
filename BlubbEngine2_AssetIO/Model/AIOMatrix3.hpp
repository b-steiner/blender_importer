#pragma once

#include "../dllconfig.hpp"
#include "AIOVector3.hpp"
#include "AIOVector2.hpp"

#include <iostream>

namespace AssetIO
{
	class AIOMatrix4;

	/// <summary>
	/// Defines a 3x3 Matrix and proviedes functions to work with them
	/// </summary>
	class _AIO_DECLSPEC AIOMatrix3
	{
		friend AIOMatrix4;

	public:
		#pragma region Constructors and Destructor

		/// <summary>
		/// Initializes a new instance of the AIOMatrix3 class and initialized it as a identity matrix
		/// </summary>
		AIOMatrix3();
		/// <summary>
		/// Initializes a new instance of the AIOMatrix3 class and initialized it as a identity matrix
		/// </summary>
		/// <param name="_m11">Value of the first column in the first row</param>
		/// <param name="_m12">Value of the second column in the first row</param>
		/// <param name="_m13">Value of the third column in the first row</param>
		/// <param name="_m21">Value of the first column in the second row</param>
		/// <param name="_m22">Value of the second column in the second row</param>
		/// <param name="_m23">Value of the third column in the second row</param>
		/// <param name="_m31">Value of the first column in the third row</param>
		/// <param name="_m32">Value of the second column in the third row</param>
		/// <param name="_m33">Value of the third column in the third row</param>
		AIOMatrix3(float _m11, float _m12, float _m13, float _m21, float _m22, float _m23, float _m31, float _m32, float _m33);
		/// <summary>
		/// Initializes a new instance of the AIOMatrix3 class
		/// </summary>
		/// <param name="_diag">Value of the main diagonal</param>
		AIOMatrix3(float _diag);
		/// <summary>
		/// Initializes a new instance of the AIOMatrix3 class
		/// </summary>
		/// <param name="_refToMat">Matrix to perfom a deep copy</param>
		AIOMatrix3(const AIOMatrix3& _refToMat);
		/// <summary>
		/// Initializes a new AIOMatrix3 object
		/// </summary>
		/// <param name="_values">Values for the matrix</param>
		AIOMatrix3(const float* _values);
		/// <summary>
		/// Releases the data from a Mat3 class
		/// </summary>
		~AIOMatrix3();

		#pragma endregion

		#pragma region Getter and Setter

		/// <summary>
		/// Sets or retrieves the value of the first column in the first row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M11() const;
		/// <summary>
		/// Sets or retrieves the value of the first column in the first row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M11(float _value);
		/// <summary>
		/// Sets or retrieves the value of the second column in the first row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M12() const;
		/// <summary>
		/// Sets or retrieves the value of the second column in the first row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M12(float _value);
		/// <summary>
		/// Sets or retrieves the value of the third column in the first row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M13() const;
		/// <summary>
		/// Sets or retrieves the value of the third column in the first row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M13(float _value);

		/// <summary>
		/// Sets or retrieves the value of the first column in the second row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M21() const;
		/// <summary>
		/// Sets or retrieves the value of the first column in the second row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M21(float _value);
		/// <summary>
		/// Sets or retrieves the value of the second column in the second row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M22() const;
		/// <summary>
		/// Sets or retrieves the value of the second column in the second row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M22(float _value);
		/// <summary>
		/// Sets or retrieves the value of the third column in the second row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M23() const;
		/// <summary>
		/// Sets or retrieves the value of the third column in the second row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M23(float _value);

		/// <summary>
		/// Sets or retrieves the value of the first column in the third row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M31() const;
		/// <summary>
		/// Sets or retrieves the value of the first column in the third row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M31(float _value);
		/// <summary>
		/// Sets or retrieves the value of the second column in the third row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M32() const;
		/// <summary>
		/// Sets or retrieves the value of the second column in the third row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M32(float _value);
		/// <summary>
		/// Sets or retrieves the value of the third column in the third row
		/// </summary>
		/// <returns>The value of the matrix element</returns>
		float M33() const;
		/// <summary>
		/// Sets or retrieves the value of the third column in the third row
		/// </summary>
		/// <param name="_value">New value for the matrix element</param>
		/// <returns>Reference to the AIOMatrix3 class</returns>
		AIOMatrix3& M33(float _value);

		/// <summary>
		/// Returns a pointer to a OpenGL matrix
		/// </summary>
		/// <returns>Pointer to the matrix</returns>
		float* GetGlMatrix() const;

		/// <summary>
		/// Defines if the matrix is stored column major or row major
		/// </summary>
		/// <returns>True if the matrix is column major, false if not</returns>
		static const bool IsColumnMajor();

		#pragma endregion

		#pragma region Mathematical Operations

		/// <summary>
		/// Transposes a AIOMatrix3 object
		/// </summary>
		/// <returns>Reference to the transposed matrix</returns>
		AIOMatrix3& Transpose();
		/// <summary>
		/// Transposes a AIOMatrix3 object
		/// </summary>
		/// <param name="_matrix">AIOMatrix3 to transpose</param>
		/// <returns>The transposed matrix</returns>
		static AIOMatrix3 Transpose(const AIOMatrix3& _matrix);
		/// <summary>
		/// Rotates a matrix along the x axis by a give angle
		/// </summary>
		/// <param name="_matrix">The matrix to rotate</param>
		/// <param name="_angle">The angle in radian</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix3& RotateX(AIOMatrix3& _matrix, float _angle);
		/// <summary>
		/// Creates a rotation matrix for a rotation along the x axis
		/// </summary>
		/// <param name="_angle">The angle to rotate in radian</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix3 RotateX(float _angle);
		/// <summary>
		/// Rotates a matrix along the y axis by a give angle
		/// </summary>
		/// <param name="_matrix">The matrix to rotate</param>
		/// <param name="_angle">The angle in radian</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix3& RotateY(AIOMatrix3& _matrix, float _angle);
		/// <summary>
		/// Creates a rotation matrix for a rotation along the y axis
		/// </summary>
		/// <param name="_angle">The angle to rotate in radian</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix3 RotateY(float _angle);
		/// <summary>
		/// Rotates a matrix along the z axis by a give angle
		/// </summary>
		/// <param name="_matrix">The matrix to rotate</param>
		/// <param name="_angle">The angle in radian</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix3& RotateZ(AIOMatrix3& _matrix, float _angle);
		/// <summary>
		/// Creates a rotation matrix for a rotation along the z axis
		/// </summary>
		/// <param name="_angle">The angle to rotate in radian</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix3 RotateZ(float _angle);
		/// <summary>
		/// Rotates a matrix along a given axis by a given value
		/// </summary>
		/// <param name="_matrix">Matrix to rotate</param>
		/// <param name="_axis">Axis to rotate along</param>
		/// <param name="_angle">The angle in radian</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix3& RotateAxis(AIOMatrix3& _matrix, const AIOVector3& _axis, float _angle);
		/// <summary>
		/// Creates a rotation matrix for rotation along a given angle
		/// </summary>
		/// <param name="_axis">Axis to rotate along</param>
		/// <param name="_angle">The angle in radian</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix3 RotateAxis(const AIOVector3& _axis, float _angle);
		/// <summary>
		/// Creates a scaling matrix
		/// </summary>
		/// <param name="_scales">The scaling factors</param>
		/// <returns>The scaling matrix</returns>
		static AIOMatrix3 Scale(const AIOVector3& _scales);
		/// <summary>
		/// Scales a matrix
		/// </summary>
		/// <param name="_matrix">The matrix to scale</param>
		/// <param name="_scale">The scaling values</param>
		/// <returns>A reference to the scaled matrix</returns>
		static AIOMatrix3& Scale(AIOMatrix3& _matrix, const AIOVector3& _scale);

		/// <summary>
		/// Creates a 2d rotation matrix
		/// </summary>
		/// <param name="_angle">The angle to rotate in radian</param>
		/// <returns>A rotation matrix</returns>
		static AIOMatrix3 Rotate2D(float _angle);
		/// <summary>
		/// Rotates a 2d transformation matrix
		/// </summary>
		/// <param name="_matrix">The matrix to rotate</param>
		/// <param name="_angle">The angle in radian</param>
		/// <returns>A reference to the rotated matrix</returns>
		static AIOMatrix3& Rotate2D(AIOMatrix3& _matrix, float _angle);
		/// <summary>
		/// Creates a 2d translation matrix
		/// </summary>
		/// <param name="_direction">The vector to translate</param>
		/// <returns>A translation matrix</returns>
		static AIOMatrix3 Translate2D(const AIOVector2& _direction);
		/// <summary>
		/// Translates a 2d transformation matrix
		/// </summary>
		/// <param name="_matrix">The matrix to translate</param>
		/// <param name="_direction">The translation vector</param>
		/// <returns>A reference to the translated matrix</returns>
		static AIOMatrix3& Translate2D(AIOMatrix3& _matrix, const AIOVector2& _direction);
		/// <summary>
		/// Creates a 2d scaling matrix
		/// </summary>
		/// <param name="_scales">The scaling factors</param>
		/// <returns>A scaling matrix</returns>
		static AIOMatrix3 Scale2D(const AIOVector2& _scales);
		/// <summary>
		/// Scales a 2d transformation matrix
		/// </summary>
		/// <param name="_matrix">The matrix to sccale</param>
		/// <param name="_scales">The scaling factors</param>
		/// <returns>A reference to the scaling matrix</returns>
		static AIOMatrix3& Scale2D(AIOMatrix3& _matrix, const AIOVector2& _scales);

		/// <summary>
		/// Inverts the matrix
		/// </summary>
		/// <returns>A references to the inverted matrix</returns>
		AIOMatrix3& Invert();
		/// <summary>
		/// Inverts a matrix
		/// </summary>
		/// <param name="_matrix">Matrix to invert</param>
		/// <returns>The inverted matrix</returns>
		static AIOMatrix3 Invert(const AIOMatrix3& _matrix);
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
		static float Det(const AIOMatrix3& _matrix);
		/// <summary>
		/// Returns the adjunct matrix
		/// </summary>
		/// <returns>The adjunct matrix</returns>
		AIOMatrix3 Adj() const;
		/// <summary>
		/// Calculates the adjunct matrix
		/// </summary>
		/// <param name="_matrix">Matrix to calculate the adjunct matrix from</param>
		/// <returns>The adjunct matrix</returns>
		static AIOMatrix3 Adj(const AIOMatrix3& _matrix);

		#pragma endregion

		#pragma region Operators

		/// <summary>
		/// Defines the multiplication of two matrices
		/// </summary>
		/// <param name="_toMultiply">The matrix to multiply with</param>
		/// <returns>The rotated matrix</returns>
		AIOMatrix3 operator * (const AIOMatrix3& _toMultiply) const;
		/// <summary>
		/// Defines the multiplication of a vector and a matrix
		/// </summary>
		/// <param name="_toMultiply">The vector</param>
		/// <returns>The multiplied vector</returns>
		AIOVector3 operator * (const AIOVector3& _toMultiply) const;
		/// <summary>
		/// Pointwise multiplication of the matrix with a scalar value
		/// </summary>
		/// <param name="_scalar">The scalar value</param>
		/// <returns>The multiplied matrix</param>
		AIOMatrix3 operator * (const float _scalar) const;
		/// <summary>
		/// Pointwise division of the matrix with a scalar value
		/// </summary>
		/// <param name="_scalar">The scalar value</param>
		/// <returns>The divided matrix</param>
		AIOMatrix3 operator / (const float _scalar) const;

		/// <summary>
		/// Defines the multiplication of two matrices
		/// </summary>
		/// <param name="_toMultiply">To matrix to multiply with</param>
		/// <returns>A reference to the multiplied matrix</returns>
		AIOMatrix3& operator *= (const AIOMatrix3& _toMultiply);
		/// <summary>
		/// Pointwise multiplication of the matrix with a scalar value
		/// </summary>
		/// <param name="_scalar">The scalar value</param>
		/// <returns>A reference to the multiplied matrix</param>
		AIOMatrix3& operator *= (const float _scalar);
		/// <summary>
		/// Pointwise division of the matrix with a scalar value
		/// </summary>
		/// <param name="_scalar">The scalar value</param>
		/// <returns>A reference to the divided matrix</param>
		AIOMatrix3& operator /= (const float _scalar);

		/// <summary>
		/// Defines the equal operator for two matrices
		/// </summary>
		/// <param name="_toCompare">The matrix to compare</param>
		/// <returns>True if the two matrices are equal</returns>
		bool operator == (const AIOMatrix3& _toCompare) const;
		/// <summary>
		/// Defines the unequal operator for two matrices
		/// </summary>
		/// <param name="_toCompare">The matrix to compare</param>
		/// <returns>True if the two matrices are unequal</returns>
		bool operator != (const AIOMatrix3& _toCompare) const;
		/// <summary>
		/// Defines the assignment operator to get a deep copy
		/// </summary>
		/// <param name="_copy">Source to copy from</param>
		/// <returns>Copied Mat3 object</returns>
		AIOMatrix3 operator = (AIOMatrix3& _copy);

		#pragma endregion

	private:
		float M(int _column, int _row);
		void M(int _column, int _row, float _value);

		float values[9];
	};
}