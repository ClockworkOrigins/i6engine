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

#ifndef __I6ENGINE_MODULES_AUDIONODE_H__
#define __I6ENGINE_MODULES_AUDIONODE_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessage.h"

#include "AL/al.h"

#include "boost/shared_ptr.hpp"

namespace i6engine {
namespace modules {

	class AudioManager;
	struct WavFile;

	/**
	 * \class AudioNode
	 * \brief represents one sound
	 */
	class AudioNode {
		friend class AudioManager;

	public:
		/**
		 * \brief constructor loading file
		 */
		AudioNode(const std::string & file, bool looping, double maxDist, const Vec3 & position, const Vec3 & direction, bool cacheable, const std::string & category);

		/**
		 * \brief constructor using cached sound
		 */
		AudioNode(boost::shared_ptr<WavFile> file, bool looping, double maxDist, const Vec3 & position, const Vec3 & direction, bool cacheable, const std::string & category);

		/**
		 * \brief destructor
		 */
		~AudioNode();

	private:
		ALuint _source;
		ALuint _buffer;
		boost::shared_ptr<WavFile> _wavFile;
		std::string _category;

		void News(const api::GameMessage::Ptr & msg);

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_AUDIONODE_H__ */

/**
 * @}
 */
