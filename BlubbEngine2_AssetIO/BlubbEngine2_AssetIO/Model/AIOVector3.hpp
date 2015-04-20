#pragma once

namespace AssetIO
{
	class AIOVector3
	{
	public:
		AIOVector3(float _x, float _y, float _z);
		AIOVector3(const AIOVector3& _other);
		~AIOVector3();

		float* Values() const;

		AIOVector3& operator=(const AIOVector3& _other);

	private:
		float values[3];
	}
}