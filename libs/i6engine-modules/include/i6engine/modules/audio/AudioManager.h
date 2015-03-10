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

#ifndef __I6ENGINE_MODULES_AUDIOMANAGER_H__
#define __I6ENGINE_MODULES_AUDIOMANAGER_H__

#include <map>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessage.h"

#include "AL/al.h"
#include "AL/alc.h"

namespace i6engine {
namespace modules {

	class AudioController;
	class AudioMailbox;
	class AudioNode;
	struct WavFile;

	class AudioManager {
		friend class AudioMailbox;

	public:
		/**
		 * \brief constructor
		 */
		AudioManager();

		/**
		 * \brief destructor
		 */
		~AudioManager();

		/**
		 * \brief ticks the audio subsystem
		 */
		void Tick();

	private:
		ALCdevice * _device;
		ALCcontext * _context;

		std::map<int64_t, boost::shared_ptr<AudioNode>> _nodes;

		std::vector<std::pair<ALuint, ALuint>> _sounds;

		std::map<std::string, boost::shared_ptr<WavFile>> _cachedSounds;

		/**
		 * \brief called by AudioMailbox with a message
		 */
		void NewsCreate(const api::GameMessage::Ptr & msg);
		void NewsUpdate(const api::GameMessage::Ptr & msg);
		void NewsDelete(const api::GameMessage::Ptr & msg);
		void NewsNodeCreate(const api::GameMessage::Ptr & msg);
		void NewsNodeUpdate(const api::GameMessage::Ptr & msg);
		void NewsNodeDelete(const api::GameMessage::Ptr & msg);

		/**
		 * \brief changes the listener
		 */
		void updateListener(const Vec3 & pos, const Quaternion & rot, const Vec3 & vel);

		/**
		 * \brief plays given sound
		 */
		void playSound(const std::string & file, double maxDistance, const Vec3 & pos, const Vec3 & dir, bool cacheable);

		/**
		 * \brief forbidden
		 */
		AudioManager(const AudioManager &) = delete;

		/**
		 * \brief forbidden
		 */
		AudioManager & operator=(const AudioManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_AUDIOMANAGER_H__ */

/**
 * @}
 */
