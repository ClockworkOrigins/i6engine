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
 * \addtogroup Audio
 * @{
 */

#ifndef __I6ENGINE_API_AUDIOFACADE_H__
#define __I6ENGINE_API_AUDIOFACADE_H__

#include <atomic>
#include <cstdint>
#include <functional>

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/configs/AudioConfig.h"

namespace i6e {
namespace api {

	class ISIXE_MODULES_API AudioFacade {
	public:
		AudioFacade();
		~AudioFacade();

		/**
		 * \brief creates a new audio node
		 */
		void createNode(int64_t comId, const std::string & f, bool l, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category);

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
		audio::SoundHandle playSound(const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category);

		/**
		 * \brief plays given sound if found and calls callback afterwards
		 */
		audio::SoundHandle playSoundWithCallback(const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category, const std::function<void(bool)> callback);

		/**
		 * \brief stops sound with this handle ID
		 */
		void stopSound(audio::SoundHandle handle);

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

	private:
		std::atomic<audio::SoundHandle> _handleCounter;

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
} /* namespace i6e */

#endif /* __I6ENGINE_API_AUDIOFACADE_H__ */

/**
 * @}
 */
