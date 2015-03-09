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
 * \addtogroup Audio
 * @{
 */

#ifndef __I6ENGINE_API_AUDIOFACADE_H__
#define __I6ENGINE_API_AUDIOFACADE_H__

#include <cstdint>

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

namespace i6engine {
namespace api {

	class ISIXE_MODULES_API AudioFacade {
	public:
		AudioFacade();
		~AudioFacade();

		/**
		 * \brief creates a new audio node
		 */
		void createNode(int64_t comId, const std::string & f, bool l, double m, const Vec3 & p, const Vec3 & d);

		/**
		 * \brief deletes an audio node
		 */
		void deleteNode(int64_t comId);

		/**
		 * \brief updates audio listener
		 */
		void updateListener(const Vec3 & position, const Quaternion & rotation, const Vec3 & velocity);

		/**
		* \brief updates audio position
		*/
		void updatePosition(int64_t comId, const Vec3 & position);

		/**
		 * \brief plays given sound once
		 */
		void playSound(const std::string & f, double m, const Vec3 & p, const Vec3 & d);

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

	private:
		/**
		 * \brief forbidden
		 */
		AudioFacade(const AudioFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		AudioFacade & operator=(const AudioFacade &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_AUDIOFACADE_H__ */

/**
 * @}
 */
