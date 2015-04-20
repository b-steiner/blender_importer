#include "AIOMatrix4.hpp"

#include <iostream>

using namespace AssetIO;

int IDX2(int _column, int _row)
{
	return 4 * _column + _row;
}

AIOMatrix4::AIOMatrix4(const float* _values)
{
	memcpy(Values(), _values, 16 * sizeof(float));
}
AIOMatrix4::AIOMatrix4(const AIOMatrix4& _other)
{
	memcpy(Values(), _other.Values(), 16 * sizeof(float));
}
AIOMatrix4::~AIOMatrix4()
{ }

float* AIOMatrix4::Values() const
{
	return (float*)(&values[0]);
}

AIOMatrix4& AIOMatrix4::operator=(const AIOMatrix4& _other)
{
	memcpy(Values(), _other.Values(), 16 * sizeof(float));
}