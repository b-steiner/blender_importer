#pragma once

namespace AssetIO
{
	class AIOVector3
	{
	public:
		AIOVector3(const float* _values);
		~AIOVector3();

		float* Values() const;

	private:
		float values[3];
	}
}