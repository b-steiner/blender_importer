#pragma once

#include <string>
#include <vector>

#include "AIOVector2.hpp"

namespace AssetIO
{
	enum class AIOInterpolationMode
	{
		Linear, Bezier
	};

	enum class AIOInterpolationTarget
	{
		LocX, LocY, LocZ,
		RotX, RotY, RotZ,
		SizeX, SizeY, SizeZ
	};

	class AIOAnimation
	{
	public:
		AIOAnimation(const std::string& _name);
		~AIOAnimation();

		std::vector<std::vector<AIOVector2>>& Points();
		std::vector<std::vector<AIOVector2>>& PrevHandle();
		std::vector<std::vector<AIOVector2>>& NextHandle();

		std::vector<AIOInterpolationMode>& InterpolationMode();

		std::vector<AIOInterpolationTarget>& Target();

	private:
		std::string name;
		std::vector<std::vector<AIOVector2>> points;
		std::vector<std::vector<AIOVector2>> prevHandles;
		std::vector<std::vector<AIOVector2>> nextHandles;
		std::vector<AIOInterpolationMode> interpolationMode;
		std::vector<AIOInterpolationTarget> target;
	};
}