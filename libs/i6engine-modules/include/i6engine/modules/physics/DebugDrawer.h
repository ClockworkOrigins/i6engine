/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * \addtogroup Physics
 * @{
 */

#ifndef __I6ENGINE_MODULES_DEBUGDRAWER_H__
#define __I6ENGINE_MODULES_DEBUGDRAWER_H__

#include "i6engine/i6engineBuildSettings.h"

#include "LinearMath/btIDebugDraw.h"

namespace i6e {
namespace modules {

	/**
	 * \class DebugDrawer
	 * This class is used to make the current physical world visible
	 * The functions are called from bullet and will send corresponding messages
	 * to the graphic for displaying different objects
	 */
	class DebugDrawer : public btIDebugDraw {
	public:
		/**
		 * \brief constructor
		 */
		DebugDrawer() : btIDebugDraw() {}

		/**
		 * \brief destructor
		 */
		~DebugDrawer() {}

		/**
		 * \brief draws a line from 'from' to 'to' with color 'col'
		 */
		void drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & col);

		/**
		 * \brief draws a line from 'from' to 'to' with color 'col'
		 * \hint toColor is currently ignored
		 */
		void drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & col, const btVector3 & /*toColor*/) {
			drawLine(from, to, col);
		}

		/**
		 * \brief currently not implemented
		 */
		void drawContactPoint(const btVector3 & a, const btVector3 & b, btScalar g, int f, const btVector3 & c);

		/**
		 * \brief currently not implemented
		 */
		void reportErrorWarning(const char * d);

		/**
		 * \brief currently not implemented
		 */
		void draw3dText(const btVector3 & a, const char * c);

		/**
		 * \brief currently not implemented
		 */
		void setDebugMode(int a);

		/**
		 * \brief returns the debug mode
		 */
		int getDebugMode() const;

	private:
		/**
		 * \brief forbidden
		 */
		DebugDrawer(const DebugDrawer &);

		/**
		 * \brief forbidden
		 */
		DebugDrawer & operator=(const DebugDrawer &);
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_DEBUGDRAWER_H__ */

/**
 * @}
 */
