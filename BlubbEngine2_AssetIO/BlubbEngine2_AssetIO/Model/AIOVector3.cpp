#include "AIOVector3.hpp"

using namespace AssetIO;

AIOVector3::AIOVector3(float _x, float _y, float _z)
{
	values[0] = _x;
	values[1] = _y;
	values[2] = _z;
}
AIOVector3::AIOVector3(const AIOVector3& _other)
{
	values[0] = _other.Values()[0];
	values[1] = _other.Values()[1];
	values[2] = _other.Values()[2];
}
AIOVector3::~AIOVector3()
{ }

float* AIOVector3::Values() const
{
	return (float*)(&values[0]);
}

AIOVector3& AIOVector3::operator=(const AIOVector3& _other)
{
	values[0] = _other.Values()[0];
	values[1] = _other.Values()[1];
	values[2] = _other.Values()[2];
}