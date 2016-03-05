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
