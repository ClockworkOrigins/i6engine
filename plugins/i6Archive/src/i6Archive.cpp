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
			std::string fileContent(decompressed, sizeof(uint32_t) + filenameLength, length - filenameLength - sizeof(uint32_t));
			_files.insert(std::make_pair(filename, fileContent));
		}
		std::cout << "Parsed " << _files.size() << " files" << std::endl;
	}

	void i6Archive::unload() {
		std::cout << "unload" << std::endl;
		// TODO
	}

	Ogre::DataStreamPtr i6Archive::open(const Ogre::String & filename, bool readOnly) const {
		std::cout << "open " << filename << std::endl;
		auto it = _files.find(filename);
		if (it == _files.end()) {
			std::cout << "error" << std::endl;
		}
		char * buffer = OGRE_ALLOC_T(char, it->second.size(), Ogre::MEMCATEGORY_GENERAL);
		memcpy(buffer, it->second.c_str(), it->second.size());
		std::cout << "open 2" << std::endl;
		Ogre::DataStream * ds = new Ogre::MemoryDataStream(filename, buffer, it->second.size(), true, true);
		std::cout << "open 3" << std::endl;
		Ogre::DataStreamPtr ret(ds);
		std::cout << "open 4" << std::endl;
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
