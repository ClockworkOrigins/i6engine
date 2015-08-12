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

#ifndef __I6ENGINE_MODULES_AUDIOCONTROLLER_H__
#define __I6ENGINE_MODULES_AUDIOCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6engine {
namespace modules {

	class AudioMailbox;
	class AudioManager;

	/**
	 * \class AudioController
	 * \brief Controls the sounds and music
	 *
	 * This class creates the audio and manages its objects.
	 */
	class ISIXE_MODULES_API AudioController : public core::ModuleController {
	public:
		/**
		 * \brief constructor
		 */
		AudioController();

		/**
		 * \brief destructor
		 */
		~AudioController();

	private:
		AudioMailbox * _mailbox;
		AudioManager * _manager;

		/**
		 * \brief This method will be called when starting the thread of the subsystem.
		 */
		void OnThreadStart() override;

		/**
		 * \brief This method will be called when shutting down the subsystem.
		 */
		void ShutDown() override;

		/**
		 * \brief This method will be called in every tick of the subsystem.
		 */
		void Tick() override;

		/**
		 * \brief forbidden
		 */
		AudioController(const AudioController &) = delete;

		/**
		 * \brief forbidden
		 */
		const AudioController & operator=(const AudioController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_AUDIOCONTROLLER_H__ */

/**
 * @}
 */
