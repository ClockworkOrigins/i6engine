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

#ifndef __I6ENGINE_MODULES_AUDIOMAILBOX_H__
#define __I6ENGINE_MODULES_AUDIOMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
namespace modules {

	class AudioController;
	class AudioManager;

	/**
	 * \class AudioMailbox
	 * \brief Handles the incoming messages on the audio channel and calls the corresponding methods.
	 */
	class AudioMailbox {
		friend class AudioController;

	protected:
		/**
		 * \brief Constructor
		 */
		explicit AudioMailbox(AudioManager * m);

		/**
		 * \brief Destructor
		 */
		~AudioMailbox() {}

		/**
		 * \brief Handles incoming messages.
		 */
		void News(const api::GameMessage::Ptr & msg) const;

	private:
		AudioManager * _manager;

		/**
		 * \brief forbidden
		 */
		AudioMailbox(const AudioMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		AudioMailbox & operator=(const AudioMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_AUDIOMAILBOX_H__ */

/**
 * @}
 */
