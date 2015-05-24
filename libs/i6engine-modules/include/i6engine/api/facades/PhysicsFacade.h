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

#ifndef __I6ENGINE_API_PHYSICSFACADE_H__
#define __I6ENGINE_API_PHYSICSFACADE_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/function.hpp"

namespace i6engine {
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
		void registerNotifyCallback(const boost::function<void(int64_t)> & f);

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
		boost::function<void(int64_t)> _notify;

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
} /* namespace i6engine */

#endif /* __I6ENGINE_API_PHYSICSFACADE_H__ */

/**
 * @}
 */
