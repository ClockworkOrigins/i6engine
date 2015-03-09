#pragma once

#include <cstdint>

#include "boost/shared_ptr.hpp"

namespace i6engine {
namespace modules {

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

	boost::shared_ptr<WavFile> loadWavFile(const std::string & file);

} /* namespace modules */
} /* namespace i6engine */
