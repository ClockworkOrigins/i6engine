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

	AudioManager::AudioManager() : _initialized(false), _device(), _context(), _nodes(), _sounds(), _cachedSounds() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		_device = alcOpenDevice(nullptr);
		if (_device) {
			_context = alcCreateContext(_device, nullptr);
		}
		if (_context) {
			alcMakeContextCurrent(_context);
			_initialized = true;
		}
		if (!_initialized) {
			ISIXE_LOG_ERROR("AudioManager", "Couldn't initialize audio subsystem. Running without audio!");
		}
	}

	AudioManager::~AudioManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_initialized) {
			alcMakeContextCurrent(nullptr);	// Make no context current
			alcDestroyContext(_context);	// Destroy the OpenAL Context
			alcCloseDevice(_device);		// Close the OpenAL Device
		}
	}

	void AudioManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_initialized) {
			for (auto it = _sounds.begin(); it != _sounds.end();) {
				ALint state;
				alGetSourcei(std::get<SoundEntry::Source>(*it), AL_SOURCE_STATE, &state);

				if (state == AL_STOPPED) {
					std::get<SoundEntry::Callback>(*it)(true);
					alDeleteSources(1, &std::get<SoundEntry::Source>(*it));	// Delete the OpenAL Source
					alDeleteBuffers(1, &std::get<SoundEntry::Buffer>(*it));	// Delete the OpenAL Buffer
					it = _sounds.erase(it);
				} else {
					it++;
				}
			}
		}
	}

	void AudioManager::NewsCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (_initialized) {
			if (type == api::audio::AudioPlaySound) {
				api::audio::Audio_PlaySound_Create * apsc = dynamic_cast<api::audio::Audio_PlaySound_Create *>(msg->getContent());

				playSound(apsc->handle, apsc->file, apsc->maxDist, apsc->position, apsc->direction, apsc->cacheable, apsc->category);
			} else if (type == api::audio::AudioPlaySoundWithCallback) {
				api::audio::Audio_PlaySoundWithCallback_Create * apsc = dynamic_cast<api::audio::Audio_PlaySoundWithCallback_Create *>(msg->getContent());

				playSound(apsc->handle, apsc->file, apsc->maxDist, apsc->position, apsc->direction, apsc->cacheable, apsc->category, apsc->callback);
			} else {
				ISIXE_THROW_MESSAGE("AudioManager", "Don't know what to do with message type " << type);
			}
		}
	}

	void AudioManager::NewsUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (_initialized) {
			if (type == api::audio::AudioListener) {
				api::audio::Audio_Listener_Update * alu = dynamic_cast<api::audio::Audio_Listener_Update *>(msg->getContent());

				updateListener(alu->position, alu->rotation, alu->velocity);
			} else {
				ISIXE_THROW_MESSAGE("AudioManager", "Don't know what to do with message type " << type);
			}
		}
	}

	void AudioManager::NewsDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (_initialized) {
			if (type == api::audio::AudioStopSound) {
				api::audio::Audio_StopSound_Delete * ass = dynamic_cast<api::audio::Audio_StopSound_Delete *>(msg->getContent());

				for (auto it = _sounds.begin(); it != _sounds.end(); it++) {
					if (std::get<SoundEntry::Handle>(*it) == ass->handle) {
						ALint state;
						alGetSourcei(std::get<SoundEntry::Source>(*it), AL_SOURCE_STATE, &state);
						if (state == AL_STOPPED) {
							std::get<SoundEntry::Callback>(*it)(true);
						} else if (state == AL_PLAYING) {
							std::get<SoundEntry::Callback>(*it)(false);
							alSourceStop(std::get<SoundEntry::Source>(*it));
						}
						alDeleteSources(1, &std::get<SoundEntry::Source>(*it));
						alDeleteBuffers(1, &std::get<SoundEntry::Buffer>(*it));
						_sounds.erase(it);
						break;
					}
				}
			} else {
				ISIXE_THROW_MESSAGE("AudioManager", "Don't know what to do with message type " << type);
			}
		}
	}

	void AudioManager::NewsNodeCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (_initialized) {
			if (type == api::audio::AudioNode) {
				api::audio::Audio_Node_Create * anc = dynamic_cast<api::audio::Audio_Node_Create *>(msg->getContent());

				auto it = _cachedSounds.find(anc->file);
				if (it != _cachedSounds.end()) {
					_nodes[msg->getContent()->getID()] = boost::make_shared<AudioNode>(it->second, anc->looping, anc->maxDist, anc->position, anc->direction, false, anc->category);
				} else {
					_nodes[msg->getContent()->getID()] = boost::make_shared<AudioNode>(anc->file, anc->looping, anc->maxDist, anc->position, anc->direction, anc->cacheable, anc->category);
					if (anc->cacheable) {
						_cachedSounds.insert(std::make_pair(anc->file, _nodes[msg->getContent()->getID()]->_wavFile));
					}
				}
			} else {
				ISIXE_THROW_MESSAGE("AudioManager", "Don't know what to do with message type " << type);
			}
		}
	}

	void AudioManager::NewsNodeUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_initialized) {
			if (_nodes.find(msg->getContent()->getID()) == _nodes.end()) {
				ISIXE_THROW_SUBSYSTEM("AudioManager", "Update message for non existend audio node!");
			}

			_nodes[msg->getContent()->getID()]->News(msg);
		}
	}

	void AudioManager::NewsNodeDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (_initialized) {
			if (type == api::audio::AudioNode) {
				_nodes.erase(msg->getContent()->getID());
			} else {
				ISIXE_THROW_MESSAGE("AudioManager", "Don't know what to do with message type " << type);
			}
		}
	}

	void AudioManager::updateListener(const Vec3 & pos, const Quaternion & rot, const Vec3 & vel) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Vec3 dir = math::rotateVector(Vec3(0.0, 0.0, 1.0), rot);
		ALfloat ListenerPos[] = { float(pos.getX()), float(pos.getY()), float(pos.getZ()) };					// Position of the listener
		ALfloat ListenerOri[] = { float(dir.getX()), float(dir.getY()), float(dir.getZ()), 0.0f, 1.0f, 0.0f };	// Orientation of the listener
		ALfloat ListenerVel[] = { float(vel.getX()), float(vel.getY()), float(vel.getZ()) };					// Velocity of the listener

		alListenerfv(AL_POSITION, ListenerPos);		// Set position of the listener
		alListenerfv(AL_ORIENTATION, ListenerOri);	// Set orientation of the listener
		alListenerfv(AL_VELOCITY, ListenerVel);		// Set velocity of the listener
	}

	void AudioManager::playSound(uint64_t handle, const std::string & file, double maxDistance, const Vec3 & pos, const Vec3 & dir, bool cacheable, const std::string & category, const std::function<void(bool)> & callback) {
		ASSERT_THREAD_SAFETY_FUNCTION
		auto it = _cachedSounds.find(file);
		boost::shared_ptr<WavFile> wh;
		if (it != _cachedSounds.end()) {
			wh = it->second;
		} else {
			try {
				wh = loadWavFile(file);
			} catch (utils::exceptions::ApiException &) {
				callback(false);
				return;
			}
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

		_sounds.push_back(std::make_tuple(source, buffer, callback, handle, category));
	}

} /* namespace modules */
} /* namespace i6engine */
