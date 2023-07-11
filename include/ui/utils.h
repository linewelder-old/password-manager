#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <optional>
#include "data/Password.h"
#include "data/Category.h"

std::optional<int> try_read_int(const std::string& string);
bool is_considered_path(const std::string& string);
unsigned int get_length_in_characters(unsigned int value);

/**
* \returns Imie pliku bez rozszerzenia.
*/
std::string get_stripped_filename(const std::filesystem::path& path);

std::vector<std::filesystem::path> get_files_with_extension(
	const std::filesystem::path& directory,
	const std::string& extension);

void print_password_table(const std::vector<CategoryRecord>& categories,
	const std::vector<PasswordRecord>& passwords);
