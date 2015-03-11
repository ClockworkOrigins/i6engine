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

#include "i6engine/modules/audio/AudioManager.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/AudioConfig.h"

#include "i6engine/modules/audio/AudioNode.h"
#include "i6engine/modules/audio/WavFile.h"

#include "AL/al.h"
#include "AL/alext.h"

#include "boost/make_shared.hpp"

namespace i6engine {
namespace modules {

	AudioManager::AudioManager() : _device(), _context(), _nodes(), _sounds(), _cachedSounds() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		_device = alcOpenDevice(nullptr);
		_context = alcCreateContext(_device, nullptr);
		alcMakeContextCurrent(_context);
	}

	AudioManager::~AudioManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		alcMakeContextCurrent(nullptr);	// Make no context current
		alcDestroyContext(_context);	// Destroy the OpenAL Context
		alcCloseDevice(_device);		// Close the OpenAL Device
	}

	void AudioManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION

		for (auto it = _sounds.begin(); it != _sounds.end(); it++) {
			ALint state;
			alGetSourcei(it->first, AL_SOURCE_STATE, &state);

			if (state == AL_STOPPED) {
				_sounds.erase(it);
				break;
			}
		}
	}

	void AudioManager::NewsCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::audio::AudioPlaySound) {
			api::audio::Audio_PlaySound_Create * apsc = dynamic_cast<api::audio::Audio_PlaySound_Create *>(msg->getContent());

			playSound(apsc->file, apsc->maxDist, apsc->position, apsc->direction, apsc->cacheable);
		}
	}

	void AudioManager::NewsUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
			uint16_t type = msg->getSubtype();

		if (type == api::audio::AudioListener) {
			api::audio::Audio_Listener_Update * alu = dynamic_cast<api::audio::Audio_Listener_Update *>(msg->getContent());

			updateListener(alu->position, alu->rotation, alu->velocity);
		}
	}

	void AudioManager::NewsDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
	}

	void AudioManager::NewsNodeCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::audio::AudioNode) {
			api::audio::Audio_Node_Create * anc = dynamic_cast<api::audio::Audio_Node_Create *>(msg->getContent());

			auto it = _cachedSounds.find(anc->file);
			boost::shared_ptr<WavFile> wh;
			if (it != _cachedSounds.end()) {
				_nodes[msg->getContent()->getID()] = boost::make_shared<AudioNode>(it->second, anc->looping, anc->maxDist, anc->position, anc->direction);
			} else {
				_nodes[msg->getContent()->getID()] = boost::make_shared<AudioNode>(anc->file, anc->looping, anc->maxDist, anc->position, anc->direction, anc->cacheable);
				if (anc->cacheable) {
					_cachedSounds.insert(std::make_pair(anc->file, _nodes[msg->getContent()->getID()]->_wavFile));
				}
			}
		}
	}

	void AudioManager::NewsNodeUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_nodes.find(msg->getContent()->getID()) == _nodes.end()) {
			ISIXE_THROW_SUBSYSTEM("AudioManager", "Update message for non existend audio node!");
		}

		_nodes[msg->getContent()->getID()]->News(msg);
	}

	void AudioManager::NewsNodeDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::audio::AudioNode) {
			_nodes.erase(msg->getContent()->getID());
		}
	}

	void AudioManager::updateListener(const Vec3 & pos, const Quaternion & rot, const Vec3 & vel) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Vec3 dir = math::rotateVector(Vec3(0.0, 0.0, -1.0), rot);
		ALfloat ListenerPos[] = { float(pos.getX()), float(pos.getY()), float(pos.getZ()) };					// Position of the listener
		ALfloat ListenerOri[] = { float(dir.getX()), float(dir.getY()), float(dir.getZ()), 0.0f, 1.0f, 0.0f };	// Orientation of the listener
		ALfloat ListenerVel[] = { float(vel.getX()), float(vel.getY()), float(vel.getZ()) };					// Velocity of the listener

		alListenerfv(AL_POSITION, ListenerPos);		// Set position of the listener
		alListenerfv(AL_ORIENTATION, ListenerOri);	// Set orientation of the listener
		alListenerfv(AL_VELOCITY, ListenerVel);		// Set velocity of the listener
	}

	void AudioManager::playSound(const std::string & file, double maxDistance, const Vec3 & pos, const Vec3 & dir, bool cacheable) {
		ASSERT_THREAD_SAFETY_FUNCTION
		auto it = _cachedSounds.find(file);
		boost::shared_ptr<WavFile> wh;
		if (it != _cachedSounds.end()) {
			wh = it->second;
		} else {
			wh = loadWavFile(file);
			if (cacheable) {
				_cachedSounds.insert(std::make_pair(file, wh));
			}
		}

		ALsizei frequency = ALsizei(wh->SampleRate);
		ALenum format = 0;

		ALuint buffer;
		ALuint source;

		alGenBuffers(1, &buffer);
		alGenSources(1, &source);

		if (wh->BitsPerSample == 8) {
			if (wh->NumberOfChanels == 1) {
				format = AL_FORMAT_MONO8;
			} else if (wh->NumberOfChanels == 2) {
				format = AL_FORMAT_STEREO8;
			}
		} else if (wh->BitsPerSample == 16) {
			if (wh->NumberOfChanels == 1) {
				format = AL_FORMAT_MONO16;
			} else if (wh->NumberOfChanels == 2) {
				format = AL_FORMAT_STEREO16;
			}
		}

		if (format == 0) {
			ISIXE_THROW_FAILURE("AudioNode", "Wrong audio format! Only MONO and STEREO in 8 and 16 bit are supported!");
		}

		alBufferData(buffer, format, wh->data, ALsizei(wh->SubChunk2Size), frequency);

		// Sound setting variables
		ALfloat SourcePos[] = { float(pos.getX()), float(pos.getY()), float(pos.getZ()) };	// Position of the source sound
		ALfloat SourceDirection[] = { float(dir.getX()), float(dir.getY()), float(dir.getZ()) };	// Position of the source sound
		ALfloat SourceVel[] = { 0.0f, 0.0f, 0.0f };										// Velocity of the source sound

		alSourcei(source, AL_BUFFER, ALint(buffer));			// Link the buffer to the source
		alSourcef(source, AL_PITCH, 1.0f);				// Set the pitch of the source
		alSourcef(source, AL_GAIN, 1.0f);				// Set the gain of the source
		alSourcef(source, AL_MAX_DISTANCE, ALfloat(maxDistance));	// Set the max distance of the source
		alSourcefv(source, AL_POSITION, SourcePos);	// Set the position of the source
		alSourcefv(source, AL_DIRECTION, SourceDirection);	// Set the direction of the source
		alSourcefv(source, AL_VELOCITY, SourceVel);	// Set the velocity of the source

		alSourcei(source, AL_LOOPING, AL_FALSE);	// Set if source is looping sound

		// PLAY 
		alSourcePlay(source);							// Play the sound buffer linked to the source

		_sounds.push_back(std::make_pair(source, buffer));
	}

} /* namespace modules */
} /* namespace i6engine */
