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

#include "i6engine/modules/audio/AudioNode.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/AudioConfig.h"

#include "i6engine/modules/audio/WavFile.h"

namespace i6e {
namespace modules {

	AudioNode::AudioNode(const std::string & file, bool looping, double maxDist, const Vec3 & position, const Vec3 & direction, bool cacheable, const std::string & category) : AudioNode(loadWavFile(file), looping, maxDist, position, direction, cacheable, category) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	AudioNode::AudioNode(boost::shared_ptr<WavFile> file, bool looping, double maxDist, const Vec3 & position, const Vec3 & direction, bool cacheable, const std::string & category) : _source(), _buffer(), _wavFile(), _category(category) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		if (cacheable) {
			_wavFile = file;
		}

		ALsizei frequency = ALsizei(file->SampleRate);
		ALenum format = 0;

		alGenBuffers(1, &_buffer);
		alGenSources(1, &_source);

		if (file->BitsPerSample == 8) {
			if (file->NumberOfChanels == 1) {
				format = AL_FORMAT_MONO8;
			} else if (file->NumberOfChanels == 2) {
				format = AL_FORMAT_STEREO8;
			}
		} else if (file->BitsPerSample == 16) {
			if (file->NumberOfChanels == 1) {
				format = AL_FORMAT_MONO16;
			} else if (file->NumberOfChanels == 2) {
				format = AL_FORMAT_STEREO16;
			}
		}

		if (format == 0) {
			ISIXE_THROW_FAILURE("AudioNode", "Wrong audio format! Only MONO and STEREO in 8 and 16 bit are supported!");
		}

		alBufferData(_buffer, format, file->data, ALsizei(file->SubChunk2Size), frequency);

		// Sound setting variables
		ALfloat SourcePos[] = { float(position.getX()), float(position.getY()), float(position.getZ()) };	// Position of the source sound
		ALfloat SourceDirection[] = { float(direction.getX()), float(direction.getY()), float(direction.getZ()) };	// Position of the source sound
		ALfloat SourceVel[] = { 0.0f, 0.0f, 0.0f };										// Velocity of the source sound

		alSourcei(_source, AL_BUFFER, ALint(_buffer));			// Link the buffer to the source
		alSourcef(_source, AL_PITCH, 1.0f);				// Set the pitch of the source
		alSourcef(_source, AL_GAIN, 1.0f);				// Set the gain of the source
		alSourcef(_source, AL_MAX_DISTANCE, ALfloat(maxDist));	// Set the max distance of the source
		alSourcefv(_source, AL_POSITION, SourcePos);	// Set the position of the source
		alSourcefv(_source, AL_DIRECTION, SourceDirection);	// Set the direction of the source
		alSourcefv(_source, AL_VELOCITY, SourceVel);	// Set the velocity of the source

		if (looping) {
			alSourcei(_source, AL_LOOPING, AL_TRUE);	// Set if source is looping sound
		} else {
			alSourcei(_source, AL_LOOPING, AL_FALSE);	// Set if source is looping sound
		}

		// PLAY 
		alSourcePlay(_source);							// Play the sound buffer linked to the source
	}

	AudioNode::~AudioNode() {
		ASSERT_THREAD_SAFETY_FUNCTION
		// Clean-up
		alDeleteSources(1, &_source);	// Delete the OpenAL Source
		alDeleteBuffers(1, &_buffer);	// Delete the OpenAL Buffer
	}

	void AudioNode::News(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (msg->getSubtype() == api::audio::AudioPosition) {
			api::audio::Audio_Position_Update * apu = dynamic_cast<api::audio::Audio_Position_Update *>(msg->getContent());

			ALfloat SourcePos[] = { float(apu->position.getX()), float(apu->position.getY()), float(apu->position.getZ()) };

			alSourcefv(_source, AL_POSITION, SourcePos);
		}
	}

} /* namespace modules */
} /* namespace i6e */
