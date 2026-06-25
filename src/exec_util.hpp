#pragma once

#include <filesystem>
#include <string_view>

std::filesystem::path get_executable_path();
std::filesystem::path get_executable_dir();
bool file_exists_in_executable_dir(std::string_view filename);
