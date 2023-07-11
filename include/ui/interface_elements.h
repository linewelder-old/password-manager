#pragma once

#include <string>
#include <filesystem>

std::filesystem::path select_file(std::string file_extension);
std::string ask_password();
