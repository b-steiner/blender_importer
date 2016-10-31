#pragma once

#include "../blender_importer.hpp"

BEGIN_BDL_BLI

	class BLI_EXPORT bli_exception
	{
		PROPERTY1(std::string, message, GET_CONST_REF);

	public:
		bli_exception(const std::string& message);
	};

END_BDL_BLI