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

#pragma once

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessage.h"

#include "AL/al.h"

namespace i6engine {
namespace modules {

	class AudioManager;
	struct WavFile;

	class AudioNode {
		friend class AudioManager;

	public:
		AudioNode(const std::string & file, bool looping, double maxDist, const Vec3 & position, const Vec3 & direction, bool cacheable);
		AudioNode(const boost::shared_ptr<WavFile> & file, bool looping, double maxDist, const Vec3 & position, const Vec3 & direction);
		~AudioNode();

	private:
		ALuint _source;
		ALuint _buffer;
		boost::shared_ptr<WavFile> _wavFile;

		void News(const api::GameMessage::Ptr & msg);

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */
