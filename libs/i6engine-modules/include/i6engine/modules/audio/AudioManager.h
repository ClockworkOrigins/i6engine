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

#ifndef __I6ENGINE_MODULES_AUDIOMANAGER_H__
#define __I6ENGINE_MODULES_AUDIOMANAGER_H__

#include <map>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessage.h"
#include "i6engine/api/configs/AudioConfig.h"

#include "AL/al.h"
#include "AL/alc.h"

namespace i6e {
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
		bool _initialized;
		ALCdevice * _device;
		ALCcontext * _context;

		std::map<int64_t, boost::shared_ptr<AudioNode>> _nodes;

		enum SoundEntry {
			Source,
			Buffer,
			Callback,
			Handle,
			Category
		};
		std::vector<std::tuple<ALuint, ALuint, std::function<void(bool)>, api::audio::SoundHandle, std::string>> _sounds;

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
		void playSound(api::audio::SoundHandle handle, const std::string & file, double maxDistance, const Vec3 & pos, const Vec3 & dir, bool cacheable, const std::string & category, const std::function<void(bool)> & callback = [](bool) {});

		/**
		 * \brief sets volume for a sound category
		 */
		void setVolume(const std::string & category, uint16_t volume);

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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_AUDIOMANAGER_H__ */

/**
 * @}
 */
