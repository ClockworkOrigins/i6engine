#include "i6engine/modules/audio/WavFile.h"

#include "i6engine/utils/Exceptions.h"

#include "boost/make_shared.hpp"

namespace i6e {
namespace modules {

	boost::shared_ptr<WavFile> loadWavFile(const std::string & file) {
		boost::shared_ptr<WavFile> wh(boost::make_shared<WavFile>());

		FILE * fp = fopen(file.c_str(), "rb");

		if (fp == nullptr) {
			ISIXE_THROW_API("WavFile", "File " << file << " couldn't be opened!");
		}

		fread(&wh->Chunk_ID, sizeof(uint8_t), 4, fp);
		fread(&wh->ChunkSize, sizeof(uint32_t), 1, fp);
		fread(&wh->Format, sizeof(uint8_t), 4, fp);

		fread(&wh->SubChunk1ID, sizeof(uint8_t), 4, fp);
		fread(&wh->SubChunk1Size, sizeof(uint32_t), 1, fp);
		fread(&wh->AudioFormat, sizeof(uint16_t), 1, fp);
		fread(&wh->NumberOfChanels, sizeof(uint16_t), 1, fp);
		fread(&wh->SampleRate, sizeof(uint32_t), 1, fp);
		fread(&wh->ByteRate, sizeof(uint32_t), 1, fp);
		fread(&wh->BlockAlignment, sizeof(uint16_t), 1, fp);
		fread(&wh->BitsPerSample, sizeof(uint16_t), 1, fp);

		while (char(wh->SubChunk2ID[0]) != 'd' || char(wh->SubChunk2ID[1]) != 'a' || char(wh->SubChunk2ID[2]) != 't' || char(wh->SubChunk2ID[3]) != 'a') {
			fread(&wh->SubChunk2ID, sizeof(uint8_t), 4, fp);
		}

		fread(&wh->SubChunk2Size, sizeof(uint32_t), 1, fp);

		wh->data = new char[wh->SubChunk2Size];

		fread(wh->data, sizeof(uint8_t), wh->SubChunk2Size, fp);

		fclose(fp);

		return wh;
	}

} /* namespace modules */
} /* namespace i6e */
