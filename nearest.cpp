#pragma warning(disable: 4530)

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<std::string> read_directory_list(const std::filesystem::path &file_path) {
	std::vector<std::string> dir_list;
	std::ifstream infile(file_path);

	if (!infile.is_open()) {
		std::ofstream outfile(file_path);
		return dir_list;
	}

	std::string line;
	while (std::getline(infile, line)) {
		dir_list.push_back(line);
	}

	infile.close();
	return dir_list;
}

void write_directory_list(const std::filesystem::path &file_path, const std::vector<std::string> &dir_list) {
	std::ofstream outfile(file_path);

	for (const auto &dir: dir_list) {
		outfile << dir << "\n";
	}

	outfile.close();
}

std::string find_matching_directory(const std::string &argument, const std::vector<std::string> &dir_list) {
	for (const auto &dir: dir_list) {
		std::string dir_basename = fs::path(dir).filename().string();
		std::string arg_lower = argument;
		std::transform(arg_lower.begin(), arg_lower.end(), arg_lower.begin(), ::tolower);
		std::string dir_basename_lower = dir_basename;
		std::transform(dir_basename_lower.begin(), dir_basename_lower.end(), dir_basename_lower.begin(), ::tolower);

		if (dir_basename_lower.find(arg_lower) == 0) {
			return dir;
		}
	}
	return "";
}

std::string find_subdirectory_with_prefix(const std::string &prefix) {
	std::string current_directory = std::filesystem::current_path().string();
	for (const auto &entry: std::filesystem::directory_iterator(current_directory)) {
		if (entry.is_directory()) {
			std::string dirname = entry.path().filename().string();
			if (dirname.find(prefix) == 0) {
				return entry.path().string();
			}
		}
	}
	return "";
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <directory>" << std::endl;
		return 1;
	}

	std::string argument = argv[1];
	auto documents_folder = fs::path(getenv("USERPROFILE")) / "Documents";
	auto list_file = documents_folder / "cd-nearest.csv";

	// Read the current list of directories
	std::vector<std::string> dir_list = read_directory_list(list_file);
	std::string target_dir;

	if (fs::path(argument).is_absolute()) {
		target_dir = argument;
	} else if (argument.find('\\') != std::string::npos) {
		target_dir = fs::absolute(argument).string();
	} else {
		target_dir = find_matching_directory(argument, dir_list);
		if (target_dir.empty()) {
			target_dir = find_subdirectory_with_prefix(argument);
			if (target_dir.empty()) {
				std::cerr << "No matching directory found for prefix: " << argument << std::endl;
				return 1;
			}
		}
	}

	// Move the chosen directory to the top of the list
	auto it = std::find(dir_list.begin(), dir_list.end(), target_dir);
	if (it != dir_list.end()) {
		dir_list.erase(it);
	}
	dir_list.insert(dir_list.begin(), target_dir);

	// Write the updated list back to the file
	write_directory_list(list_file, dir_list);

	// Print the target directory
	std::cout << target_dir << std::endl;

	return 0;
}
