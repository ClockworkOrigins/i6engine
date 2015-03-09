/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \addtogroup Physics
 * @{
 */

#ifndef __I6ENGINE_MODULES_DEBUGDRAWER_H__
#define __I6ENGINE_MODULES_DEBUGDRAWER_H__

#include "i6engine/i6engineBuildSettings.h"

#include "LinearMath/btIDebugDraw.h"

namespace i6engine {
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
		void drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & col, const btVector3 & toColor) {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_DEBUGDRAWER_H__ */

/**
 * @}
 */
