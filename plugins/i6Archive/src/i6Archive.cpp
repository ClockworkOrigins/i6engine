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

#include "i6Archive.h"

#include <fstream>
#include <regex>
#include <thread>

#include "clockUtils/compression/Compression.h"
#include "clockUtils/compression/algorithm/HuffmanGeneric.h"

namespace i6engine {
namespace plugins {

	void i6Archive::load() {
		// one asset is split into
		// 4 Byte (length of compressed string) | 4 Byte (length of filename) | filename | compressed data
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
			uint32_t filenameLength = 0;
			in.read(reinterpret_cast<char *>(&filenameLength), sizeof(uint32_t));
			char * buffer = new char[size_t(filenameLength)];
			in.read(buffer, filenameLength);
			std::string filename(buffer, filenameLength);
			_files.insert(std::make_pair(filename, std::make_pair(size_t(in.tellg()), length)));
			in.ignore(length);
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
		std::ifstream in(mName, std::ifstream::ate | std::ifstream::binary);
		if (!in.good()) {
			return Ogre::DataStreamPtr();
		}
		in.seekg(it->second.first, std::ios::beg);
		if (!in.eof()) {
			char * buffer = new char[size_t(it->second.second)];
			in.read(buffer, it->second.second);
			std::string compressed(buffer, it->second.second);
			clockUtils::compression::Compression<clockUtils::compression::algorithm::HuffmanGeneric> decompressor;
			std::string decompressed = decompressor.decompress(compressed);
			char * realData = new char[decompressed.size()];
			memcpy(realData, decompressed.c_str(), decompressed.size());
			Ogre::DataStream * ds = new Ogre::MemoryDataStream(filename, realData, decompressed.size(), true, true);
			Ogre::DataStreamPtr ret(ds);
			return ret;
		}
		return Ogre::DataStreamPtr();
	}

	Ogre::StringVectorPtr i6Archive::list(bool recursive, bool dirs) {
		return find("*", recursive, dirs);
	}

	Ogre::FileInfoListPtr i6Archive::listFileInfo(bool recursive, bool dirs) {
		return findFileInfo("*", recursive, dirs);
	}

	Ogre::StringVectorPtr i6Archive::find(const Ogre::String & pattern, bool recursive, bool dirs) {
		Ogre::StringVectorPtr ret(OGRE_NEW_T(Ogre::StringVector, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
		for (std::map<std::string, std::pair<size_t, size_t>>::const_iterator it = _files.begin(); it != _files.end(); it++) {
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
		for (std::map<std::string, std::pair<size_t, size_t>>::const_iterator it = _files.begin(); it != _files.end(); it++) {
			std::regex regex(std::regex_replace(pattern, std::regex("\\*\\."), ".*."));
			if (std::regex_match(it->first, regex)) {
				Ogre::FileInfo info;
				info.archive = this;
				info.filename = it->first;
				info.basename = it->first;
				info.compressedSize = it->second.second;
				//info.uncompressedSize = it->second.second;
				info.path = "";
				ret->push_back(info);
			}
		}
		return ret;
	}

} /* namespace plugins */
} /* namespace i6engine */
