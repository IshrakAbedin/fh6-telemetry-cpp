#include "exec_util.hpp"

#include <stdexcept>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

std::filesystem::path get_executable_path()
{
#if defined(_WIN32)
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer);
#elif defined(__linux__)
    char buffer[4096];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0';
        return std::filesystem::path(buffer);
    }
    throw std::runtime_error("Failed to get executable path");
#elif defined(__APPLE__)
    char buffer[4096];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0)
        return std::filesystem::canonical(buffer);
    throw std::runtime_error("Failed to get executable path");
#else
#error "Unsupported platform"
#endif
}

std::filesystem::path get_executable_dir()
{
    return get_executable_path().parent_path();
}

bool file_exists_in_executable_dir(std::string_view filename)
{
    auto dir = get_executable_dir();
    auto target = dir / filename;
    return std::filesystem::exists(target);
}
