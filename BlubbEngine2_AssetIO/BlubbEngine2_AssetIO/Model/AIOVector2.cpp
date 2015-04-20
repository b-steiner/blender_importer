#include "AIOVector2.hpp"

using namespace AssetIO;

AIOVector2::AIOVector2(float _x, float _y)
{
	values[0] = _x;
	values[1] = _y;
}
AIOVector2::AIOVector2(const AIOVector2& _value)
{
	values[0] = _value.Values()[0];
	values[1] = _value.Values()[1];
}
AIOVector2::~AIOVector2()
{ }

float* AIOVector2::Values() const
{
	return (float*)(&values[0]);
}

AIOVector2& AIOVector2::operator=(const AIOVector2& _other)
{
	values[0] = _other.Values()[0];
	values[1] = _other.Values()[1];
}