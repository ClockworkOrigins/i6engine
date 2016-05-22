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

#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#include <cstring>
#endif

int main(int argc, char ** argv) {
	std::vector<std::string> search;
	search.push_back("");
	std::queue<std::string> analyse;
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	analyse.push(argv[1]);
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	analyse.push(std::regex_replace(argv[1], std::regex("\\\\"), "/"));
#endif
	for (int i = 2; i < argc; i++) {
		search.push_back(std::string(argv[i]));
	}
	std::map<std::string, size_t> mm;
	mm[analyse.front()] = 0;
	std::cout << "digraph {" << std::endl;
	std::cout << "0 [label=\"" << analyse.front() << "\"];" << std::endl;
	std::set<std::string> already;
	while (!analyse.empty()) {
		std::string f = analyse.front();
		analyse.pop();
		if (already.find(f) != already.end()) {
			continue;
		}
		already.insert(f);
		size_t kk = mm[f];
		size_t i;
		for (i = 0; i < search.size(); i++) {
			FILE * fp = fopen((search[i] + f).c_str(), "r");
			if (fp != NULL) {
				char buffer[2048];
				while (fgets(buffer, 2048, fp) != NULL) {
					if (!strncmp("#include \"", buffer, 10)) {
						int start = 10, end = 10;
						while (buffer[end] != '\"') end++;
						std::string inc(buffer + start, buffer + end);
						analyse.push(inc);
						if (mm.find(inc) == mm.end()) {
							mm.insert(std::make_pair(inc, mm.size()));
							std::cout << mm[inc] << " [label=\"" << inc << "\"];" << std::endl;
						}
						std::cout << kk << " -> " << mm[inc] << ";" << std::endl;
					}
				}
				fclose(fp);
				break;
			}
		}
		if (i == search.size()) {
			std::cerr << "Can't find '" << f << "'" << std::endl;
		}
	}
	std::cout << "}" << std::endl;
	return 0;
}
