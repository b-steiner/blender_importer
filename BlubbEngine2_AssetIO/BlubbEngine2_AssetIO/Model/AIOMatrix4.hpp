#pragma once

namespace AssetIO
{
	class AIOMatrix4
	{
	public:
		AIOMatrix4(const float* _values);
		~AIOMatrix4();

		float* Values() const;

	private:
		float values[16];
	};
}