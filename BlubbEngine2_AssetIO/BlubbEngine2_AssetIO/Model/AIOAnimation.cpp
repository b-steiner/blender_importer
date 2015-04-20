#include "AIOAnimation.hpp"

using namespace AssetIO;

AIOAnimation::AIOAnimation(const std::string& _name) : name(_name)
{ }
AIOAnimation::~AIOAnimation()
{ }

std::vector<std::vector<AIOVector2>>& AIOAnimation::Points()
{
	return points;
}
std::vector<std::vector<AIOVector2>>& AIOAnimation::PrevHandle()
{
	return prevHandles;
}
std::vector<std::vector<AIOVector2>>& AIOAnimation::NextHandle()
{
	return nextHandles;
}

std::vector<AIOInterpolationMode>& AIOAnimation::InterpolationMode()
{
	return interpolationMode;
}

std::vector<AIOInterpolationTarget>& AIOAnimation::Target()
{
	return target;
}