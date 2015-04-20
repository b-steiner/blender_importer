#pragma once

namespace AssetIO
{
	class AIOVector4
	{
	public:
		AIOVector4(float _x, float _y, float _z, float _w);
		AIOVector4(const AIOVector4& _other);
		~AIOVector4();

		float* Values() const;

		AIOVector4& operator=(const AIOVector4& _other);

	private:
		float values[3];
	}
}