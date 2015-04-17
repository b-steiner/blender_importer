#pragma once

namespace AssetIO
{
	class AIOVector4
	{
	public:
		AIOVector4(const float* _values);
		~AIOVector4();

		float* Values() const;

	private:
		float values[3];
	}
}