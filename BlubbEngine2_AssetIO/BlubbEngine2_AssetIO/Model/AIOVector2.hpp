#pragma once

namespace AssetIO
{
	class AIOVector2
	{
	public:
		AIOVector2(const float* _values);
		~AIOVector2();

		float* Values() const;

	private:
		float values[2];
	}
}