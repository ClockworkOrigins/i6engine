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

#ifndef __I6ENGINE_API_AUDIOCONFIG_H__
#define __I6ENGINE_API_AUDIOCONFIG_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessageStruct.h"

namespace i6engine {
namespace api {
namespace audio {
	enum AudioMessageTypes {
		AudioReset = 0,
		AudioNode,
		AudioListener,
		AudioPosition,
		AudioUnlock,
		AudioPlaySound,
		AudioPlaySoundWithCallback,
		AudioStopSound,
		AudioMessageTypesCount
	};

	/**
	 * \brief creates a new audio node
	 */
	typedef struct Audio_Node_Create : GameMessageStruct {
		std::string file;
		bool looping;
		double maxDist;
		Vec3 position;
		Vec3 direction;
		bool cacheable;
		Audio_Node_Create(int64_t c, const std::string & f, bool l, double m, const Vec3 & p, const Vec3 & d, bool ca) : GameMessageStruct(c, -1), file(f), looping(l), maxDist(m), position(p), direction(d), cacheable(ca) {
		}
		Audio_Node_Create * copy() {
			return new Audio_Node_Create(*this);
		}
	} Audio_Node_Create;

	/**
	 * \brief deletes an audio node
	 */
	typedef struct Audio_Node_Delete : GameMessageStruct {
		Audio_Node_Delete(int64_t c) : GameMessageStruct(c, -1) {
		}
		Audio_Node_Delete * copy() {
			return new Audio_Node_Delete(*this);
		}
	} Audio_Node_Delete;

	/**
	 * \brief updates sound listener
	 */
	typedef struct Audio_Listener_Update : GameMessageStruct {
		Vec3 position;
		Quaternion rotation;
		Vec3 velocity;
		Audio_Listener_Update(const Vec3 & p, const Quaternion & r, const Vec3 & v) : GameMessageStruct(), position(p), rotation(r), velocity(v) {
		}
		Audio_Listener_Update * copy() {
			return new Audio_Listener_Update(*this);
		}
	} Audio_Listener_Update;

	/**
	* \brief updates nodes position
	*/
	typedef struct Audio_Position_Update : GameMessageStruct {
		Vec3 position;
		Audio_Position_Update(int64_t c, const Vec3 & p) : GameMessageStruct(c, -1), position(p) {
		}
		Audio_Position_Update * copy() {
			return new Audio_Position_Update(*this);
		}
	} Audio_Position_Update;

	/**
	 * \brief plays given sound once
	 */
	typedef struct Audio_PlaySound_Create : GameMessageStruct {
		uint64_t handle;
		std::string file;
		double maxDist;
		Vec3 position;
		Vec3 direction;
		bool cacheable;
		Audio_PlaySound_Create(uint64_t h, const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool c) : GameMessageStruct(), handle(h), file(f), maxDist(m), position(p), direction(d), cacheable(c) {
		}
		Audio_PlaySound_Create * copy() {
			return new Audio_PlaySound_Create(*this);
		}
	} Audio_PlaySound_Create;

	/**
	 * \brief plays given sound with callback
	 */
	typedef struct Audio_PlaySoundWithCallback_Create : GameMessageStruct {
		uint64_t handle;
		std::string file;
		double maxDist;
		Vec3 position;
		Vec3 direction;
		bool cacheable;
		std::function<void(bool)> callback;
		Audio_PlaySoundWithCallback_Create(uint64_t h, const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool c, const std::function<void(bool)> & cb) : GameMessageStruct(), handle(h), file(f), maxDist(m), position(p), direction(d), cacheable(c), callback(cb) {
		}
		Audio_PlaySoundWithCallback_Create * copy() {
			return new Audio_PlaySoundWithCallback_Create(*this);
		}
	} Audio_PlaySoundWithCallback_Create;

	/**
	 * \brief stops given sound
	 */
	typedef struct Audio_StopSound_Delete : GameMessageStruct {
		uint64_t handle;
		Audio_StopSound_Delete(uint64_t h) : GameMessageStruct(), handle(h) {
		}
		Audio_StopSound_Delete * copy() {
			return new Audio_StopSound_Delete(*this);
		}
	} Audio_StopSound_Delete;

} /* namespace audio */
} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_AUDIOCONFIG_H__ */

/**
 * @}
 */
