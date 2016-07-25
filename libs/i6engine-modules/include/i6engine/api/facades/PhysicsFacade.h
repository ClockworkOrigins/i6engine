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

#ifndef __I6ENGINE_API_PHYSICSFACADE_H__
#define __I6ENGINE_API_PHYSICSFACADE_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/function.hpp"

namespace i6e {
namespace math {
	class i6eVector;
} /* namespace math */
namespace api {

	/**
	 * \class PhysicsFacade
	 * \brief Physics specific functions for the game which targets the whole world - not a single object
	 */
	class ISIXE_MODULES_API PhysicsFacade {
	public:
		/**
		 * \brief Contructor
		 */
		PhysicsFacade() : _notify() {}

		/**
		 * \brief Destructor
		 */
		~PhysicsFacade() {}

		/**
		 * \brief Sets the gravity of the world
		 * \param[in] i6eVector vec3Gravity - The "force" direction. The usual gravity is (0, -y, 0) where -y is the value of the desired gravity (9.8 in rl)
		 */
		void setGravity(const math::i6eVector & vec3Gravity) const;

		/**
		 * \brief register callback for notifying IDs, call only from PhysicsController
		 */
		void registerNotifyCallback(const std::function<void(int64_t)> & f);

		/**
		 * \brief notifies new id in subsystem
		 */
		void notifyNewID(int64_t id);

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

		/**
		 * \brief pauses the subsystem
		 */
		void pause() const;

		/**
		 * \brief unpauses the subsystem
		 */
		void unpause() const;

	private:
		std::function<void(int64_t)> _notify;

		/**
		 * \brief forbidden
		 */
		PhysicsFacade(const PhysicsFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		PhysicsFacade & operator=(const PhysicsFacade &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_PHYSICSFACADE_H__ */

/**
 * @}
 */
