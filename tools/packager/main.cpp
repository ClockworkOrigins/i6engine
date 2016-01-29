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

#include <fstream>
#include <iostream>
#include <queue>
#include <string>

#include "i6engine/utils/i6eString.h"

#include "boost/filesystem.hpp"

#include "clockUtils/compression/Compression.h"
#include "clockUtils/compression/algorithm/HuffmanGeneric.h"

void packFile(std::ofstream & out, const std::string & file) {
	std::ifstream in(file, std::ifstream::ate | std::ifstream::binary);
	if (!in.good()) {
		std::cout << "Couldn't open file " << file << std::endl;
		return;
	}
	auto size = in.tellg();
	in.seekg(0, std::ios::beg);
	char * buffer = new char[size_t(size)];

	in.read(buffer, size);

	in.close();

	std::string filename = i6engine::utils::split(file, "/").back();
	filename = i6engine::utils::split(file, "\\").back();

	char * lBuffer = new char[sizeof(uint32_t)];
	uint32_t filenameLength = filename.length();
	lBuffer = reinterpret_cast<char *>(&filenameLength);

	clockUtils::compression::Compression<clockUtils::compression::algorithm::HuffmanGeneric> compressor;
	std::string compressed = compressor.compress(std::string(lBuffer, sizeof(uint32_t)) + filename + std::string(buffer, size_t(size)));
	uint64_t length = uint64_t(size);
	out.write(reinterpret_cast<char *>(&length), sizeof(uint64_t));
	out.write(compressed.c_str(), compressed.size());
	out.flush();
}

int main(int argc, char ** argv) {
	argc--;
	argv++;

	if (argc < 2) {
		std::cerr << "packager requires at least two arguments: outfileName (without extension) and input directory/file" << std::endl;
		return 1;
	}

	std::ofstream out(std::string(argv[0]) + ".i6arch", std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

	std::queue<std::string> directories;

	for (int i = 1; i < argc; i++) {
		directories.push(argv[i]);
	}

	while (!directories.empty()) {
		std::string dir = directories.front();
		directories.pop();
		try {
			boost::filesystem::directory_iterator iter(dir), dirEnd;
			while (iter != dirEnd) {
				if (boost::filesystem::is_regular_file(*iter)) {
					std::string file = iter->path().string();
					packFile(out, file);
				} else if (boost::filesystem::is_directory(*iter)) {
					std::string path = iter->path().string();
					directories.push(path);
				}
				iter++;
			}
		} catch (boost::filesystem::filesystem_error) {
			// entry not existing OR entry is file
			if (boost::filesystem::is_regular_file(dir)) {
				packFile(out, dir);
			}
		}
	}
	out.close();

	return 0;
}

