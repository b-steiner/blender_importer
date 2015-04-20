#include "AIOVector4.hpp"

using namespace AssetIO;

AIOVector4::AIOVector4(float _x, float _y, float _z, float _w)
{
	values[0] = _x;
	values[1] = _y;
	values[2] = _z;
	values[3] = _w;
}
AIOVector4::AIOVector4(const AIOVector4& _other)
{
	values[0] = _other.Values()[0];
	values[1] = _other.Values()[1];
	values[2] = _other.Values()[2];
	values[3] = _other.Values()[3];
}
AIOVector4::~AIOVector4()
{ }

float* AIOVector4::Values() const
{
	return (float*)(&values[0]);
}

AIOVector4& AIOVector4::operator=(const AIOVector4& _other)
{
	values[0] = _other.Values()[0];
	values[1] = _other.Values()[1];
	values[2] = _other.Values()[2];
	values[3] = _other.Values()[3];
}