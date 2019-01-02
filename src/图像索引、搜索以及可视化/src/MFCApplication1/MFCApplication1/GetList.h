#ifndef GETLIST_H
#define GETLIST_H

#include <vector>
#include <string>

extern std::vector<std::string> get_files_inDirectory(const std::string& _path, const std::string& _filter);

#endif // !GETLIST_H
