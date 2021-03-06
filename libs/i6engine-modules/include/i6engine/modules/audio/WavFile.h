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

#ifndef __I6ENGINE_MODULES_WAVFILE_H__
#define __I6ENGINE_MODULES_WAVFILE_H__

#include <cstdint>

#include "boost/shared_ptr.hpp"

namespace i6e {
namespace modules {

	/**
	 * \brief parsed wave file
	 */
	typedef struct WavFile {
		uint8_t Chunk_ID[4];
		uint32_t ChunkSize;
		uint8_t Format[4];

		uint8_t SubChunk1ID[4];
		uint32_t SubChunk1Size;
		uint16_t AudioFormat;
		uint16_t NumberOfChanels;
		uint32_t SampleRate;
		uint32_t ByteRate;
		uint16_t BlockAlignment;
		uint16_t BitsPerSample;

		uint8_t SubChunk2ID[4];
		uint32_t SubChunk2Size;

		//Everything else is data. We note it's offset
		char * data = nullptr;

		WavFile() {
		}

		~WavFile() {
			delete data;
		}
	} WavFile;

	/**
	 * \brief loads a wav file
	 */
	boost::shared_ptr<WavFile> loadWavFile(const std::string & file);

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_WAVFILE_H__ */

/**
 * @}
 */
