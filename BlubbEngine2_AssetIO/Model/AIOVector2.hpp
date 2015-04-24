#pragma once

#include "../dllconfig.hpp"

#include <iostream>

namespace AssetIO
{
	/// <summary>
	/// Descripes a two-dimensional vector and provies functions for working with them
	/// </summary>
	class _AIO_DECLSPEC AIOVector2
	{

	public:
		#pragma region Constructors & Destructor

		/// <summary>
		/// Initializes a new instance of the Vec2 class
		/// </summary>
		AIOVector2();
		/// <summary>
		/// Initializes an new instance of the Vec2 class
		/// </summary>
		/// <param name="_x">x-coordinate</param>
		/// <param name="_y">y-coordinate</param>
		AIOVector2(float _x, float _y);
		/// <summary>
		/// Initializes a new instance of the AIOVector2 class
		/// <param name="_values">Array of length 2 [x, y]</param>
		/// </summary>
		AIOVector2(float* _values);
		/// <summary>
		/// Initializes an new instance of the Vec2 class
		/// </summary>
		/// <param name="_refToVec">Vector to copy</param>
		AIOVector2(const AIOVector2& _refToVec);
		/// <summary>
		/// Releases the data from a Vec2 class
		/// </summary>
		~AIOVector2();

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
		/// <returns>A reference to the Vec2 class</returns>
		inline AIOVector2& X(float _x);
		/// <summary>
		/// Sets or retrieves the value of the y-coordinate
		/// </summary>
		/// <returns>The value of the y-coordinate</returns>
		inline float Y() const;
		/// <summary>
		/// Sets or retrieves the value of the y-coordinate
		/// </summary>
		/// <param name="_y">New value for the y-coordinate</param>
		/// <returns>A reference to the Vec2 class</returns>
		inline AIOVector2& Y(float _y);
		/// <summary>
		/// Returns a pointer to a OpenGL vector
		/// </summary>
		/// <returns>Pointer to OpenGL vector</returns>
		inline float* GetGlVector() const;

		#pragma endregion

		#pragma region Mathematical Methods

		/// <summary>
		/// Normalizes the vector
		/// </summary>
		/// <returns>Normalized vector</returns>
		inline AIOVector2& Normalize();
		/// <summary>
		/// Normalizes a Vec2 class
		/// </summary>
		/// <param name="_vector">A Vec2 to normalize</param>
		/// <returns>The normalized vector</returns>
		static AIOVector2 Normalize(const AIOVector2 &_vector);

		/// <summary>
		/// Returns the lenght of the vector
		/// </summary>
		/// <returns>The lenght of the vector</returns>
		inline float Length() const;
		/// <summary>
		/// Returns the lenght of a Vec2
		/// </summary>
		/// <param name="_vector">The vector</param>
		/// <returns>The lenght of the vector</returns>
		static float Length(const AIOVector2 &_vector);

		/// <summary>
		/// Returns the componentwise maximum vector
		/// </summary>
		/// <param name="_v1">First vector</param>
		/// <param name="_v2">Second vector</param>
		/// <returns>The maximum vector</returns>
		static AIOVector2 ComponentMax(const AIOVector2& _v1, const AIOVector2& _v2);
		/// <summary>
		/// Returns the componentwise minimum vector
		/// </summary>
		/// <param name="_v1">First vector</param>
		/// <param name="_v2">Second vector</param>
		/// <returns>The minimum vector</returns>
		static AIOVector2 ComponentMin(const AIOVector2& _v1, const AIOVector2& _v2);
		/// <summary>
		/// Performs a linear interpolation between two vectors. The weights are normalized before the interpolation
		/// </summary>
		/// <param name="_v1">First vector</param>
		/// <param name="_v2">Second vector</param>
		/// <param name="_a1">Weight for the first vector</param>
		/// <param name="_a2">Weight for the second vector</param>
		/// <returns>The interpolated vector</returns>
		static AIOVector2 Interpolate(const AIOVector2& _v1, const AIOVector2& _v2, float _a1, float _a2);

		#pragma endregion

		#pragma region Operators

		/// <summary>
		/// Defines a pointwise addition for Vec2 classes
		/// </summary>
		/// <param name="_toAdd">Vector to add</param>
		/// <returns>The added Vector</returns>
		inline AIOVector2 operator + (const AIOVector2& _toAdd) const;
		/// <summary>
		/// Subtracts to Vec2 classes pointwise
		/// </summary>
		/// <param name="_toSubtract">Vector to subtract</param>
		/// <returns>The Subtracted Vector</returns>
		inline AIOVector2 operator - (const AIOVector2& _toSubtract) const;
		/// <summary>
		/// Multiplies the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to multiply with</param>
		/// <returns>The multiplied vector</returns>
		inline AIOVector2 operator * (float _scalar) const;
		/// <summary>
		/// Divides the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to divide with</param>
		/// <returns>The divided vector</returns>
		inline AIOVector2 operator / (float _scalar) const;
		/// <summary>
		/// Defines a pointwise addition for Vec2 classes
		/// </summary>
		/// <param name="_toAdd">Vector to add</param>
		/// <returns>The added Vector</returns>
		inline AIOVector2& operator += (const AIOVector2& _toAdd);
		/// <summary>
		/// Subtracts to Vec2 classes pointwise
		/// </summary>
		/// <param name="_toSubtract">Vector to subtract</param>
		/// <returns>The Subtracted Vector</returns>
		inline AIOVector2& operator -= (const AIOVector2& _toSubtract);
		/// <summary>
		/// Multiplies the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to multiply with</param>
		/// <returns>Multiplied vector</returns>
		inline AIOVector2& operator *= (float _scalar);
		/// <summary>
		/// Divides the vector with a scalar
		/// </summary>
		/// <param name="_scalar">Scalar to divide with</param>
		/// <returns>Divided vector</returns>
		inline AIOVector2& operator /= (float _scalar);
		/// <summary>
		/// Defines the equal-operator for two Vec2 objects
		/// </summary>
		/// <param name="_toCompare">Vec2 class to check with</param>
		/// <returns>true if vectors are equal, false otherwise</returns>
		bool operator == (const AIOVector2& _toCompare) const;
		/// <summary>
		/// Defines the unequal-operator for two Vec2 objects
		/// </summary>
		/// <param name="_toCompare">Vec2 class to check with</param>
		/// <returns>true if vectors are unequal, false otherwise</returns>
		bool operator != (const AIOVector2& _toCompare) const;
		/// <summary>
		/// Defines the assignment operator to get a deep copy
		/// </summary>
		/// <param name="_copy">Source to copy from</param>
		/// <returns>Copied Vec2 object</returns>
		AIOVector2& operator = (const AIOVector2& _copy);

		#pragma endregion

	private:
		float values[2];
	};
}