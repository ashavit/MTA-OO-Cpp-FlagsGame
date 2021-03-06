#include "FileManager.h"
#include <string>
#include "Commons.h"

using namespace std;

FileManager& FileManager::sharedInstance() {
	static FileManager instance{};
	return instance;
}

void FileManager::loadAvailableFiles(const string& path) {

	regulatePath(path);

	char buffer[4096];
	string data;
	string command = "2>NUL dir /a-d /b ";
	command.append(fileNameWithPath("*", BOARD_FILE_EXTENSION));

	FILE* fp = _popen(command.c_str(), "r");

	while (fgets(buffer, 4095, fp)) {
		data.append(string(buffer));
	}
	_pclose(fp);

	parseFileNames(data);
	filesLeft = boardFiles.size();
}

string FileManager::nextFileName() {
	static auto next = boardFiles.begin();
	string name;
	if (next != boardFiles.end()) {
		name.assign(*next);
		++next;
		--filesLeft;
	}
	return name;
}

bool FileManager::hasMoreBoards() const {
	return filesLeft;
}

string FileManager::fileNameWithPath(const string fileName, const std::string ext) const {
	string fullPath = regulatedPath + fileName + "." + ext;
	return fullPath;
}

void FileManager::regulatePath(const string path) {
	// Check if relative path or full path - if contains :
	if (path.empty()) {
		regulatedPath = ".\\";
	}
	else if (path.find(':') == std::string::npos) {
		// Relative Path
		regulatedPath = ".";
		if (path.substr(0, 1).compare("\\") != 0) {
			regulatedPath.append("\\");
		}
		regulatedPath.append(path);
		if (path.substr(path.length() - 1, 1).compare("\\") != 0) {
			regulatedPath.append("\\");
		}
	}
	else {
		// Full path
		regulatedPath = path;
		if (path.substr(path.length() - 1, 1).compare("\\") != 0) {
			regulatedPath.append("\\");
		}
	}
}

// comparison, not case sensitive.
bool compare_nocase(const string& first, const string& second) {
	unsigned int i = 0;
	while ((i < first.length()) && (i < second.length())) {
		if (tolower(first[i]) < tolower(second[i])) return true;
		else if (tolower(first[i]) > tolower(second[i])) return false;
		++i;
	}
	return (first.length() < second.length());
}

void FileManager::parseFileNames(string& data) {
	int end;

	while ((end = data.find('\n')) != string::npos) {
		string fileName;
		fileName.assign(data, 0, end);
		boardFiles.push_back(fileName);
		data.erase(0, end + 1);
	}

	// Remove extension from name
	for (auto& name : boardFiles) {
		name.erase(name.find_last_of("."));
	}
	boardFiles.sort(compare_nocase);
}
