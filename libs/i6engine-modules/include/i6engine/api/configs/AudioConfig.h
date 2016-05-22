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

#ifndef __I6ENGINE_API_AUDIOCONFIG_H__
#define __I6ENGINE_API_AUDIOCONFIG_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessageStruct.h"

namespace i6e {
namespace api {
namespace audio {

	typedef uint64_t SoundHandle;

#define INVALID_SOUND UINT64_MAX

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
		std::string category;
		Audio_Node_Create(int64_t c, const std::string & f, bool l, double m, const Vec3 & p, const Vec3 & d, bool ca, const std::string & cat) : GameMessageStruct(c, -1), file(f), looping(l), maxDist(m), position(p), direction(d), cacheable(ca), category(cat) {
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
		SoundHandle handle;
		std::string file;
		double maxDist;
		Vec3 position;
		Vec3 direction;
		bool cacheable;
		std::string category;
		Audio_PlaySound_Create(SoundHandle h, const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool c, const std::string & cat) : GameMessageStruct(), handle(h), file(f), maxDist(m), position(p), direction(d), cacheable(c), category(cat) {
		}
		Audio_PlaySound_Create * copy() {
			return new Audio_PlaySound_Create(*this);
		}
	} Audio_PlaySound_Create;

	/**
	 * \brief plays given sound with callback
	 */
	typedef struct Audio_PlaySoundWithCallback_Create : GameMessageStruct {
		SoundHandle handle;
		std::string file;
		double maxDist;
		Vec3 position;
		Vec3 direction;
		bool cacheable;
		std::string category;
		std::function<void(bool)> callback;
		Audio_PlaySoundWithCallback_Create(SoundHandle h, const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool c, const std::string & cat, const std::function<void(bool)> & cb) : GameMessageStruct(), handle(h), file(f), maxDist(m), position(p), direction(d), cacheable(c), category(cat), callback(cb) {
		}
		Audio_PlaySoundWithCallback_Create * copy() {
			return new Audio_PlaySoundWithCallback_Create(*this);
		}
	} Audio_PlaySoundWithCallback_Create;

	/**
	 * \brief stops given sound
	 */
	typedef struct Audio_StopSound_Delete : GameMessageStruct {
		SoundHandle handle;
		Audio_StopSound_Delete(SoundHandle h) : GameMessageStruct(), handle(h) {
		}
		Audio_StopSound_Delete * copy() {
			return new Audio_StopSound_Delete(*this);
		}
	} Audio_StopSound_Delete;

} /* namespace audio */
} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_AUDIOCONFIG_H__ */

/**
 * @}
 */
