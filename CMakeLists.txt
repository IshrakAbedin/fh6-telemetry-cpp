cmake_minimum_required(VERSION 3.23)
project(FH6-Telemetry LANGUAGES CXX)

# CRITICAL FOR INTELLISENSE
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Asio
include(FetchContent)

FetchContent_Declare(
    asio_backend
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-38-0 # Latest 16-June-2026
)
FetchContent_MakeAvailable(asio_backend)

# Interface for Asio

add_library(asio_standalone INTERFACE)

# Expose headers for Asio
target_include_directories(asio_standalone INTERFACE
    ${asio_backend_SOURCE_DIR}/include
)

# Tell Asio not to look for Boost
target_compile_definitions(asio_standalone INTERFACE ASIO_STANDALONE)

# Network programming requires system threads on Linux/macOS
find_package(Threads REQUIRED)
target_link_libraries(asio_standalone INTERFACE Threads::Threads)

# Socket libraru for Windows
if(WIN32)
  # Windows requires Winsock libraries for networking functionality
  target_link_libraries(asio_standalone INTERFACE ws2_32 mswsock)
endif()

# Executable target
add_executable(fh6-telemetry-server)

target_sources(fh6-telemetry-server
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

target_include_directories(fh6-telemetry-server PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(fh6-telemetry-server PRIVATE asio_standalone)
