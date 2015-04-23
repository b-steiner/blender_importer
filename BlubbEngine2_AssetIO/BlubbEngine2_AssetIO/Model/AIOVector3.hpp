#pragma once

#include "../dllconfig.hpp"

#include <iostream>


namespace AssetIO
{
	class AIOVector4;

	/// <summary>
	/// Descripes a three-dimensional vector and provides functions for working with them
	/// </summary>
	class _AIO_DECLSPEC AIOVector3
	{
	public:
		#pragma region Constructors & Destructor

		/// <summary>
		/// Initializes a new instance of the AIOVector3 class
		/// </summary>
		AIOVector3();
		/// <summary>
		/// Initializes a new instance of the AIOVector3 class
		/// </summary>
		/// <param name="_x">x-coordinate</param>
		/// <param name="_y">y-coordinate</param>
		/// <param name="_z">z-coordinate</param>
		AIOVector3(float _x, float _y, float _z);
		/// <summary>
		/// Initializes a new instance of the AIOVector3 class
		/// </summary>
		/// <param name="_refToVec">Vector to copy</param>
		AIOVector3(const AIOVector3 &_refToVec);
		/// <summary>
		/// Initializes a new instance of the AIOVector3 class
		/// </summary>
		/// <param name="_values">Values for the AIOVector3</param>
		AIOVector3(float* _values);
		/// <summary>
		/// Initializes a new instance of the AIOVector3 class, uses the X, Y and Z index of the specified Vec4f for the eqivalent indeces in the new Vec3
		/// </summary>
		/// <param name="_vec4">The <see cref="Vec4f" /> to copy the X, Y and Z component from</param>
		AIOVector3(const AIOVector4 &_vec4);
		/// <summary>
		/// Releases the data from a AIOVector3 class
		/// </summary>
		~AIOVector3();

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
		/// <returns>A reference to the AIOVector3 class</returns>
		inline AIOVector3& X(float _x);
		/// <summary>
		/// Sets or retrieves the value of the y-coordinate
		/// </summary>
		/// <returns>The value of the y-coordinate</returns>
		inline float Y() const;
		/// <summary>
		/// Sets or retrieves the value of the y-coordinate
		/// </summary>
		/// <param name="_y">New value for the y-coordinate</param>
		/// <returns>A reference to the AIOVector3 class</returns>
		inline AIOVector3& Y(float _y);
		/// <summary>
		/// Sets or retrieves the value of the z-coordinate
		/// </summary>
		/// <returns>The value of the z-coordinate</returns>
		inline float Z() const;
		/// <summary>
		/// Sets or retrieves the value of the z-coordinate
		/// </summary>
		/// <param name="_z">New value for the z-coordinate</param>
		/// <returns>A reference to the AIOVector3 class</returns>
		inline AIOVector3& Z(float _z);
		/// <summary>
		/// Returns a pointer to a OpenGL vector
		/// </summary>
		/// <returns>Pointer to OpenGL vector</returns>
		inline float* GetGlVector() const;
		/// <summary>
		/// Constructs a 4-dimensional vector containing [x, y, z, 1]
		/// </summary>
		/// <returns>The 4-dimensional vector</returns>
		inline AIOVector4 GetVec4() const;

		#pragma endregion

		#pragma region Mathematical Methods

		/// <summary>
		/// Normalizes the vector
		/// </summary>
		/// <returns>Normalized vector</returns>
		inline AIOVector3& Normalize();
		/// <summary>
		/// Normalizes a AIOVector3 class
		/// </summary>
		/// <param name="_vector">A AIOVector3 to normalize</param>
		/// <returns>The normalized vector</returns>
		static AIOVector3 Normalize(const AIOVector3 &_vector);

		/// <summary>
		/// Returns the lenght of the vector
		/// </summary>
		/// <returns>The lenght of the vector</returns>
		float inline Length() const;
		/// <summary>
		/// Returns the lenght of a AIOVector3
		/// </summary>
		/// <param name="_vector">The vector</param>
		/// <returns>The lenght of the vector</returns>
		static float Length(AIOVector3 &_vector);

		/// <summary>
		/// Calculates the dot product of two AIOVector3 objects
		/// </summary>
		/// <param name="_left">Left vector</param>
		/// <param name="_right">Right vector</param>
		/// <returns>The dot product of the two vectors</returns>
		static float Dot(const AIOVector3 &_left, const AIOVector3 &_right);
		/// <summary>
		/// Calculates the cross product of two AIOVector3 objects
		/// </summary>
		/// <param name="_left">Left vector</param>
		/// <param name="_right">Right vector</param>
		/// <returns>The cross product of the two vectors</returns>
		static AIOVector3 Cross(const AIOVector3 &_left, const AIOVector3 &_right);
		/// <summary>
		/// Returns the componentwise maximum vector
		/// </summary>
		/// <param name="_v1">First vector</param>
		/// <param name="_v2">Second vector</param>
		/// <returns>The maximum vector</returns>
		static AIOVector3 ComponentMax(const AIOVector3& _v1, const AIOVector3& _v2);
		/// <summary>
		/// Returns the componentwise minimum vector
		/// </summary>
		/// <param name="_v1">First vector</param>
		/// <param name="_v2">Second vector</param>
		/// <returns>The minimum vector</returns>
		static AIOVector3 ComponentMin(const AIOVector3& _v1, const AIOVector3& _v2);
		/// <summary>
		/// Performs a linear interpolation between two vectors. The weights are normalized before the interpolation
		/// </summary>
		/// <param name="_v1">First vector</param>
		/// <param name="_v2">Second vector</param>
		/// <param name="_a1">Weight for the first vector</param>
		/// <param name="_a2">Weight for the second vector</param>
		/// <returns>The interpolated vector</returns>
		static AIOVector3 Interpolate(const AIOVector3& _v1, const AIOVector3& _v2, float _a1, float _a2);

		#pragma endregion

		#pragma region Operators

		/// <summary>
		/// Defines a pointwise addition for AIOVector3 classes
		/// </summary>
		/// <param name="_toAdd">Vector to add</param>
		/// <returns>The added Vector</returns>
		inline AIOVector3 operator + (const AIOVector3 &_toAdd) const;
		/// <summary>
		/// Defines a pointwise addition of a AIOVector3 with a <see cref="Vec4f" />
		/// </summary>
		/// <param name="_toAdd">Vector to add</param>
		/// <returns>The added Vector</returns>
		inline AIOVector3 operator + (const AIOVector4 &_toAdd) const;

		/// <summary>
		/// Subtracts to AIOVector3 classes pointwise
		/// </summary>
		/// <param name="_toSubtract">Vector to subtract</param>
		/// <returns>The Subtracted Vector</returns>
		inline AIOVector3 operator - (const AIOVector3 &_toSubtract) const;
		/// <summary>
		/// Defines a pointwise subtraction of a AIOVector3 with a <see cref="Vec4f" />
		/// </summary>
		/// <param name="_toSubtract">Vector to subtract</param>
		/// <returns>The subtracted Vector</returns>
		inline AIOVector3 operator - (const AIOVector4 &_toSubtract) const;

		/// <summary>
		/// Multiplies the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to multiply with</param>
		/// <returns>The multiplied vector</returns>
		inline AIOVector3 operator * (float _scalar) const;
		/// <summary>
		/// Divides the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to divide by</param>
		/// <returns>The divided vector</returns>
		inline AIOVector3 operator / (float _scalar) const;

		/// <summary>
		/// Defines a pointwise addition for AIOVector3 classes
		/// </summary>
		/// <param name="_toAdd">Vector to add</param>
		/// <returns>The added Vector</returns>
		inline AIOVector3& operator += (const AIOVector3 &_toAdd);
		/// <summary>
		/// Defines a pointwise addition of a AIOVector3 with a <see cref="Vec4f" />
		/// </summary>
		/// <param name="_toAdd">Vector to add</param>
		/// <returns>The added Vector</returns>
		inline AIOVector3& operator += (const AIOVector4 &_toAdd);

		/// <summary>
		/// Subtracts to AIOVector3 classes pointwise
		/// </summary>
		/// <param name="_toSubtract">Vector to subtract</param>
		/// <returns>The Subtracted Vector</returns>
		inline AIOVector3& operator -= (const AIOVector3 &_toSubtract);
		/// <summary>
		/// Defines a pointwise subtraction of a AIOVector3 with a <see cref="Vec4f" />
		/// </summary>
		/// <param name="_toSubtract">Vector to subtract</param>
		/// <returns>The subtracted Vector</returns>
		inline AIOVector3& operator -= (const AIOVector4 &_toSubtract);

		/// <summary>
		/// Multiplies the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to multiply with</param>
		/// <returns>Multiplied vector</returns>
		inline AIOVector3& operator *= (float _scalar);
		/// <summary>
		/// Divides the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to divide with</param>
		/// <returns>Divided vector</returns>
		inline AIOVector3& operator /= (float _scalar);

		/// <summary>
		/// Defines the equal-operator for two AIOVector3 objects
		/// </summary>
		/// <param name="_toCompare">AIOVector3 class to check with</param>
		/// <returns>true if vectors are equal, false otherwise</returns>
		bool operator == (const AIOVector3 &_toCompare) const;
		/// <summary>
		/// Defines the unequal-operator for two AIOVector3 objects
		/// </summary>
		/// <param name="_toCompare">AIOVector3 class to check with</param>
		/// <returns>true if vectors are unequal, false otherwise</returns>
		bool operator != (const AIOVector3 &_toCompare) const;

		/// <summary>
		/// Defines the assignment operator to get a deep copy
		/// </summary>
		/// <param name="_copy">Source to copy from</param>
		/// <returns>Copied AIOVector3 object</returns>
		AIOVector3& operator = (const AIOVector3 &_copy);

		#pragma endregion

	private:
		float values[3];
	};
}