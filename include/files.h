#ifndef FILES_H
#define FILES_H
#include <string>
#include <fstream>

inline std::string load_string(std::string path) {
	std::string content;
	std::fstream f(path.c_str(), std::ios::in);
	if(!f.is_open())
		return "";
	std::string l;
	while(!f.eof()) {
		std::getline(f, l);
		content.append(l + "\n");
	}
	f.close();
	return content;
}
#endif
