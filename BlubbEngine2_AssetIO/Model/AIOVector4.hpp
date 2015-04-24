#pragma once

#include "../dllconfig.hpp"
#include <iostream>
#include "AIOVector3.hpp"

#define AIOVECTOR4_ALIGN_16 _declspec(align(16))

namespace AssetIO
{
	/// <summary>
	/// Descripes a four-dimensional (homogenous) vector and provides functions for working with them
	/// </summary>
	class AIOVECTOR4_ALIGN_16 _AIO_DECLSPEC AIOVector4
	{
	public:
		#pragma region Constructors and Destructor

		/// <summary>
		/// Initializes a new instance of the AIOVector4 class
		/// </summary>
		AIOVector4();
		/// <summary>
		/// Initializes a new instance of the AIOVector4 class
		/// <param name="_values">Array of length 4 [x, y, z, w]</param>
		/// </summary>
		AIOVector4(float* _values);
		/// <summary>
		/// Initializes a new instance of the AIOVector4 class
		/// </summary>
		/// <param name="_x">x-coordinate</param>
		/// <param name="_y">y-coordinate</param>
		/// <param name="_z">z-coordinate</param>
		AIOVector4(float _x, float _y, float _z);
		/// <summary>
		/// Initializes a new instance of the AIOVector4 class
		/// </summary>
		/// <param name="_x">x-coordinate</param>
		/// <param name="_y">y-coordinate</param>
		/// <param name="_z">z-coordinate</param>
		/// <param name="_w">w-coordinate</param>
		AIOVector4(float _x, float _y, float _z, float _w);
		/// <summary>
		/// Initializes a new instance of the Vec4 class
		/// </summary>
		/// <param name="_refToVec">Vector to perform a deep copy</param>
		AIOVector4(const AIOVector4 &_refToVec);
		/// <summary>
		/// Initializes a new instance of the Vec4 class
		/// </summary>
		/// <param name="_refToVec">Vector to perform a deep copy</param>
		/// <param name="_w">Value for the W component</param>
		AIOVector4(const AIOVector3&_refToVec, float _w);
		/// <summary>
		/// Defines the assignment operator to get a deep copy
		/// </summary>
		/// <param name="_rhs">Source to copy from</param>
		/// <returns>Copied AIOVector4 object</returns>
		AIOVector4& operator = (const AIOVector4 &_rhs);
		/// <summary>
		/// Releases the data from a Vec4 class
		/// </summary>
		~AIOVector4();

		#pragma endregion

		#pragma region Getter and Setter

		/// <summary>
		/// Sets or retrieves the value of the x-coordinate
		/// </summary>
		/// <returns>The value of the x-coordinate</returns>
		inline float X() const;
		/// <summary>
		/// Sets or retrieves the value of the x-coordinate
		/// </summary>
		/// <param name="_x">New value for the x-coordinate</param>
		/// <returns>A reference to the AIOVector4 class</returns>
		inline AIOVector4& X(float _x);
		/// <summary>
		/// Sets or retrieves the value of the y-coordinate
		/// </summary>
		/// <returns>The value of the y-coordinate</returns>
		inline float Y(void) const;
		/// <summary>
		/// Sets or retrieves the value of the y-coordinate
		/// </summary>
		/// <param name="_y">New value for the y-coordinate</param>
		/// <returns>A reference to the AIOVector4 class</returns>
		inline AIOVector4& Y(float _y);
		/// <summary>
		/// Sets or retrieves the value of the z-coordinate
		/// </summary>
		/// <returns>The value of the z-coordinate</returns>
		inline float Z(void) const;
		/// <summary>
		/// Sets or retrieves the value of the z-coordinate
		/// </summary>
		/// <param name="_z">New value for the z-coordinate</param>
		/// <returns>A reference to the AIOVector4 class</returns>
		inline AIOVector4& Z(float _z);
		/// <summary>
		/// Sets or retrieves the value of the w-coordinate
		/// </summary>
		/// <returns>The value of the w-coordinate</returns>
		inline float W(void) const;
		/// <summary>
		/// Sets or retrieves the value of the w-coordinate
		/// </summary>
		/// <param name="_w">New value for the w-coordinate</param>
		/// <returns>A reference to the AIOVector4 class</returns>
		inline AIOVector4& W(float _w);
		/// <summary>
		/// Returns a pointer to a OpenGL vector
		/// </summary>
		/// <returns>Pointer to OpenGL vector</returns>
		inline float* GetGlVector() const;

		/// <summary>
		/// Returns a three-dimensional vector containing [x, y, z]
		/// </summary>
		/// <returns>XYZ-Vector</returns>
		inline AIOVector3 GetVec3() const;

		#pragma endregion

		#pragma region Mathematical Operations

		/// <summary>
		/// Projects the Vector to hyper plane w=1
		/// </summary>
		/// <returns>The projected vector</returns>
		AIOVector4& Homogenize();

		/// <summary>
		/// Projects the Vector to hyper plane w=1
		/// </summary>
		/// <param name="_vector">The vector to homogenize</param>
		/// <returns>The projected vector</returns>
		static AIOVector4 Homogenize(const AIOVector4& _vector);

		/// <summary>
		/// Normalizes the vector disregarding its last axis. 
		/// </summary>
		/// <returns>The normalized vector</returns>
		AIOVector4& Normalize3();
		/// <summary>
		/// Normalizes a AIOVector4 object copy disregarding its last axis.
		/// </summary>
		/// <param name="_vector">A AIOVector4 to copy and normalize</param>
		/// <returns>The normalized vector</returns>
		static AIOVector4 Normalize3(const AIOVector4 &_vector);
		/// <summary>
		/// Normalizes the vector on all four axes. 
		/// </summary>
		/// <returns>The normalized vector</returns>
		AIOVector4& Normalize();
		/// <summary>
		/// Normalizes a AIOVector4 object copy on all four axes.
		/// </summary>
		/// <param name="_vector">A AIOVector4 to copy and normalize</param>
		/// <returns>The normalized vector</returns>
		static AIOVector4 Normalize(const AIOVector4 &_vector);

		/// <summary>
		/// Returns the length of the vector disregarding its last axis
		/// </summary>
		/// <returns>The length of the vector</returns>
		float Length3() const;
		/// <summary>
		/// Returns the lenght of a AIOVector4 disregarding its last axis
		/// </summary>
		/// <param name="_vector">The vector</param>
		/// <returns>The lenght of the vector</returns>
		static float Length3(const AIOVector4 &_vector);
		/// <summary>
		/// Returns the length of the vector on all axes
		/// </summary>
		/// <returns>The length of the vector</returns>
		float Length() const;
		/// <summary>
		/// Returns the lenght of a AIOVector4 on all axes
		/// </summary>
		/// <param name="_vector">The vector</param>
		/// <returns>The length of the vector</returns>
		static float Length(const AIOVector4 &_vector);
			
		/// <summary>
		/// Calculates the dot product of two AIOVector4 objects disregarding their last axes
		/// </summary>
		/// <param name="_left">Left vector</param>
		/// <param name="_right">Right vector</param>
		/// <returns>The dot product of the two vectors</returns>
		static float Dot3(const AIOVector4 &_left, const AIOVector4 &_right);
		/// <summary>
		/// Calculates the dot product of two AIOVector4 objects
		/// </summary>
		/// <param name="_left">Left vector</param>
		/// <param name="_right">Right vector</param>
		/// <returns>The dot product of the two vectors</returns>
		static float Dot(const AIOVector4& _left, const AIOVector4& _right);
		/// <summary>
		/// Calculates the cross product of two AIOVector4 objects disregarding their last axes
		/// </summary>
		/// <param name="_left">Left vector</param>
		/// <param name="_right">Right vector</param>
		/// <returns>The cross product of the two vectors</returns>
		static AIOVector4 Cross3(const AIOVector4 &_left, const AIOVector4 &_right);

		/// <summary>
		/// Calculates the componentwise minimum of two vectors
		/// </summary>
		/// <param name="_v1">The first vector</param>
		/// <param name="_v2">The second vector</param>
		/// <returns>The componentwise minimum</returns>
		static AIOVector4 ComponentMin(const AIOVector4& _v1, const AIOVector4& _v2);
		/// <summary>
		/// Calculates the componentwise maximum of two vectors
		/// </summary>
		/// <param name="_v1">The first vector</param>
		/// <param name="_v2">The second vector</param>
		/// <returns>The componentwise minimum</returns>
		static AIOVector4 ComponentMax(const AIOVector4& _v1, const AIOVector4& _v2);

		/// <summary>
		/// Performs a linear interpolation between two vectors. The weights are normalized before the interpolation
		/// </summary>
		/// <param name="_v1">First vector</param>
		/// <param name="_v2">Second vector</param>
		/// <param name="_a1">Weight for the first vector</param>
		/// <param name="_a2">Weight for the second vector</param>
		/// <returns>The interpolated vector</returns>
		static AIOVector4 Interpolate(const AIOVector4& _v1, const AIOVector4& _v2, float _a1, float _a2);

		#pragma endregion

		#pragma region Operators

		/// <summary>
		/// Defines a pointwise addition for AIOVector4 classes
		/// </summary>
		/// <param name="_rhs">Vector to add</param>
		/// <returns>The added Vector</returns>
		AIOVector4 operator + (const AIOVector4 &_rhs) const;
		/// <summary>
		/// Defines a pointwise addition for a AIOVector4 with a Vec3f classes (only first three components)
		/// </summary>
		/// <param name="_rhs">Vector to add</param>
		/// <returns>The added Vector</returns>
		AIOVector3 operator + (const AIOVector3 &_rhs) const;
		/// <summary>
		/// Subtracts to AIOVector4 classes pointwise
		/// </summary>
		/// <param name="_rhs">Vector to subtract</param>
		/// <returns>The Subtracted Vector</returns>
		AIOVector4 operator - (const AIOVector4 &_rhs) const;
		/// <summary>
		/// Subtracts a Vec3f from a AIOVector4 classes pointwise.
		/// </summary>
		/// <param name="_rhs">Vector to subtract</param>
		/// <returns>The Subtracted Vector</returns>
		AIOVector3 operator - (const AIOVector3 &_rhs) const;
		/// <summary>
		/// Multiplies the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to multiply with</param>
		/// <returns>The multiplied vector</returns>
		AIOVector4 operator * (float _scalar) const;
		/// <summary>
		/// Divides the vector by a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to divide by</param>
		/// <returns>The divided vector</returns>
		AIOVector4 operator / (float _scalar) const;

		/// <summary>
		/// Defines a pointwise addition for AIOVector4 classes
		/// </summary>
		/// <param name="_rhs">Vector to add</param>
		/// <returns>The added Vector</returns>
		AIOVector4& operator += (const AIOVector4 &_rhs);
		/// <summary>
		/// Defines a pointwise addition of a AIOVector4 and a <see cref="Vec3f" />
		/// </summary>
		/// <param name="_rhs">Vector to add</param>
		/// <returns>The added Vector</returns>
		AIOVector4& operator += (const AIOVector3 &_rhs);

		/// <summary>
		/// Subtracts to AIOVector4 classes pointwise
		/// </summary>
		/// <param name="_rhs">Vector to subtract</param>
		/// <returns>The Subtracted Vector</returns>
		AIOVector4& operator -= (const AIOVector4 &_rhs);
		/// <summary>
		/// Defines a pointwise subtraction of a AIOVector4 and a <see cref="Vec3f" />
		/// </summary>
		/// <param name="_rhs">Vector to subtract</param>
		/// <returns>The subtracted Vector</returns>
		AIOVector4& operator -= (const AIOVector3 &_rhs);

		/// <summary>
		/// Multiplies the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to multiply with</param>
		/// <returns>Multiplied vector</returns>
		AIOVector4& operator *= (const float _scalar);
		/// <summary>
		/// Divides the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to divide with</param>
		/// <returns>Divided vector</returns>
		AIOVector4& operator /= (const float _scalar);

		/// <summary>
		/// Defines the equal-operator for two AIOVector4 objects
		/// </summary>
		/// <param name="_toCompare">AIOVector4 class to check with</param>
		/// <returns>true if vectors are equal, false otherwise</returns>
		bool operator == (const AIOVector4 &_toCompare) const;
		/// <summary>
		/// Defines the unequal-operator for two AIOVector4 objects
		/// </summary>
		/// <param name="_toCompare">AIOVector4 class to check with</param>
		/// <returns>true if vectors are unequal, false otherwise</returns>
		bool operator != (const AIOVector4 &_toCompare) const;

		#pragma endregion

	private:
		float values[4];
	};
}