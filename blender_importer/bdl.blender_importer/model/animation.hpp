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
#include "../util/bli_vector2.hpp"

BEGIN_BDL_BLI

	//! Enumeration with different interpolation types
	enum class interpolation_mode
	{
		//! Constant interpolation
		constant = 0,
		//! Linear interpolation
		linear = 1,
		//! Bezier interpolation
		bezier = 2
	};

	//! Enumeration with different targets for animations
	enum class animation_target
	{
		locX, locY, locZ,
		rotX, rotY, rotZ,
		sizeX, sizeY, sizeZ
	};

	/*! \brief Stores data for an objects animation
	 *
	 * \author bdl
	 *
	 * Each animation consists of several animation curves (first index in points, prev_handles, etc.).
	 * Each curve consits of several control points stored in points and for each of these points a previouse handle and a next handle
	 * When the interpolation for a point pi is set to bezier, the control points are given by [points[pi], next_handles[pi], prev_handles[pi + 1], points[pi + 1].
	 * The corresponding handles are only valid when the interpolation is set to bezier.
	 */
	class BLI_EXPORT animation
	{
		//! Stores the name of the animation
		PROPERTY1(std::string, name, GET_CONST_REF);
		//! Stores the control points
		PROPERTY1(std::vector<std::vector<bli_vector2>>, points, GET_REF);
		//! Stores the previouse handle for each point
		PROPERTY1(std::vector<std::vector<bli_vector2>>, prev_handles, GET_REF);
		//! Stores the next handle for each point
		PROPERTY1(std::vector<std::vector<bli_vector2>>, next_handles, GET_REF);
		//! Stores the interpolation mode for the curve segment between the current point and the next point
		PROPERTY1(std::vector<std::vector<interpolation_mode>>, interpolation_mode, GET_REF);
		//! Stores which target the curve affects
		PROPERTY1(std::vector<animation_target>, target, GET_REF);

	public:
		/*! \brief Initializes a new instance of the animation class
		 *
		 * \param name The name of the animation
		 */
		animation(const std::string& name);
		/*! \brief Releases all data associated with an instance of the animation
		 */
		~animation();
	};

END_BDL_BLI