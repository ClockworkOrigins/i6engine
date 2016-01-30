#include "i6Archive.h"

#include <fstream>
#include <regex>
#include <thread>

#include "clockUtils/compression/Compression.h"
#include "clockUtils/compression/algorithm/HuffmanGeneric.h"

namespace i6engine {
namespace plugins {

	void i6Archive::load() {
		std::ifstream in(mName, std::ifstream::ate | std::ifstream::binary);
		if (!in.good()) {
			return;
		}
		in.seekg(0, std::ios::beg);
		while (!in.eof()) {
			uint32_t length = 0;
			in.read(reinterpret_cast<char *>(&length), sizeof(uint32_t)).eof();
			if (in.eof()) {
				break;
			}
			char * buffer = new char[size_t(length)];
			in.read(buffer, length);
			std::string compressed(buffer, length);
			clockUtils::compression::Compression<clockUtils::compression::algorithm::HuffmanGeneric> decompressor;
			std::string decompressed = decompressor.decompress(compressed);
			uint32_t filenameLength;
			memcpy(&filenameLength, decompressed.c_str(), sizeof(uint32_t));
			std::string filename(decompressed, sizeof(uint32_t), filenameLength);
			std::string fileContent(decompressed, sizeof(uint32_t) + filenameLength, decompressed.length() - filenameLength - sizeof(uint32_t));
			_files.insert(std::make_pair(filename, fileContent));
		}
	}

	void i6Archive::unload() {
		_files.clear();
	}

	Ogre::DataStreamPtr i6Archive::open(const Ogre::String & filename, bool readOnly) const {
		auto it = _files.find(filename);
		if (it == _files.end()) {
			return Ogre::DataStreamPtr();
		}
		Ogre::DataStream * ds = new Ogre::MemoryDataStream(filename, const_cast<char *>(it->second.c_str()), it->second.size(), false, true);
		Ogre::DataStreamPtr ret(ds);
		return ret;
	}

	Ogre::StringVectorPtr i6Archive::list(bool recursive, bool dirs) {
		return find("*", recursive, dirs);
	}

	Ogre::FileInfoListPtr i6Archive::listFileInfo(bool recursive, bool dirs) {
		return findFileInfo("*", recursive, dirs);
	}

	Ogre::StringVectorPtr i6Archive::find(const Ogre::String & pattern, bool recursive, bool dirs) {
		Ogre::StringVectorPtr ret(OGRE_NEW_T(Ogre::StringVector, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
		for (std::map<std::string, std::string>::const_iterator it = _files.begin(); it != _files.end(); it++) {
			std::regex regex(std::regex_replace(pattern, std::regex("\\*"), ".*"));
			if (std::regex_match(it->first, regex)) {
				ret->push_back(it->first);
			}
		}
		return ret;
	}

	bool i6Archive::exists(const Ogre::String & filename) {
		return _files.find(filename) != _files.end();
	}

	Ogre::FileInfoListPtr i6Archive::findFileInfo(const Ogre::String & pattern, bool recursive, bool dirs) const {
		Ogre::FileInfoListPtr ret(OGRE_NEW_T(Ogre::FileInfoList, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
		for (std::map<std::string, std::string>::const_iterator it = _files.begin(); it != _files.end(); it++) {
			std::regex regex(std::regex_replace(pattern, std::regex("\\*\\."), ".*."));
			if (std::regex_match(it->first, regex)) {
				Ogre::FileInfo info;
				info.archive = this;
				info.filename = it->first;
				info.basename = it->first;
				info.compressedSize = it->second.size();
				info.uncompressedSize = it->second.size();
				info.path = "";
				ret->push_back(info);
			}
		}
		return ret;
	}

} /* namespace plugins */
} /* namespace i6engine */
