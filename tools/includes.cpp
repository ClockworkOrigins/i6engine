#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <cstdio>
#include <cstring>
#include <set>

using namespace std;

int main(int argc, char ** argv) {
	vector<string> search;
	search.push_back("");
	queue<string> analyse;
	analyse.push(string(argv[1]));
	for(int i = 2; i < argc; i++) {
		search.push_back(string(argv[i]));
	}
	map<string, int> mm;
	mm[analyse.front()] = 0;
	cout << "digraph {" << endl;
	cout << "0 [label=\"" << analyse.front() << "\"];" << endl;
	set<string> already;
	while (!analyse.empty()) {
		string f = analyse.front();
		analyse.pop();
		if (already.find(f) != already.end()) {
			continue;
		}
		already.insert(f);
		int kk = mm[f];
		int i;
		for(i = 0; i < search.size(); i++) {
			FILE * fp = fopen((search[i] + f).c_str(), "r");
			if (fp != NULL) {
				char buffer[2048];
				while(fgets(buffer, 2048, fp) != NULL) {
					if (!strncmp("#include \"", buffer, 10)) {
						int start = 10, end = 10;
						while(buffer[end] != '\"') end++;
						string inc(buffer + start, buffer + end);
						analyse.push(inc);
						if (mm.find(inc) == mm.end()) {
							mm.insert(make_pair(inc, mm.size()));
							cout << mm[inc] << " [label=\"" << inc << "\"];" << endl;
						}
						cout << kk << " -> " << mm[inc] << ";" << endl;
					}
				}
				fclose(fp);
				break;
			}
		}
		if (i == search.size()) {
			cerr << "Can't find '" << f << "'" << endl;
		}
	}
	cout << "}" << endl;
}
