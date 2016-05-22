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

	std::string filename = i6e::utils::split(file, "/").back();
	filename = i6e::utils::split(filename, "\\").back();

	uint32_t filenameLength = uint32_t(filename.length());
	clockUtils::compression::Compression<clockUtils::compression::algorithm::HuffmanGeneric> compressor;
	std::string compressed = compressor.compress(std::string(buffer, size_t(size)));
	uint32_t length = uint32_t(compressed.length());
	out.write(reinterpret_cast<char *>(&length), sizeof(uint32_t));
	out.write(reinterpret_cast<char *>(&filenameLength), sizeof(uint32_t));
	out.write(filename.c_str(), filename.size());
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

	uint32_t counter = 0;
	while (!directories.empty()) {
		std::string dir = directories.front();
		directories.pop();
		try {
			boost::filesystem::directory_iterator iter(dir), dirEnd;
			while (iter != dirEnd) {
				if (boost::filesystem::is_regular_file(*iter)) {
					std::string file = iter->path().string();
					packFile(out, file);
					counter++;
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
				counter++;
			}
		}
	}
	out.close();
	std::cout << "Packed " << counter << " files" << std::endl;

	return 0;
}

