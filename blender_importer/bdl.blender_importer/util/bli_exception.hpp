/**************************************************************************************
**
** Copyright (C) 2016 Bernhard Steiner
**
** This file is part of the blender_importer library
**
** This product is licensed under the GNU General Public License version 3.
** The license is as published by the Free Software Foundation published at
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** Alternatively, this product is licensed under the GNU Lesser General Public
** License version 3 for non-commercial use. The license is as published by the
** Free Software Foundation published at https://www.gnu.org/licenses/lgpl-3.0.html.
**
** Feel free to contact us if you have any questions about licensing or want
** to use the library in a commercial closed source product.
**
**************************************************************************************/


#pragma once

#include "../blender_importer.hpp"

BEGIN_BDL_BLI

	/*! \brief An exception thrown by the importer
	 *
	 * \author bdl
	 */
	class BLI_EXPORT bli_exception
	{
		//! Stores the error message
		PROPERTY1(std::string, message, GET_CONST_REF);

	public:
		/*! \brief Initializes a new instance of the bli_exception class
		 *
		 * \param message The error message
		 */
		bli_exception(const std::string& message);
	};

END_BDL_BLI