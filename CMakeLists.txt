cmake_minimum_required(VERSION 3.23)
project(FH6-Telemetry LANGUAGES CXX)

# Required for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch dependencies
include(FetchContent)

# Fetch Asio
FetchContent_Declare(
    asio_backend
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-38-0
    GIT_SHALLOW TRUE
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

# Socket library for Windows
if(WIN32)
  # Windows requires Winsock libraries for networking functionality
  target_link_libraries(asio_standalone INTERFACE ws2_32 mswsock)
endif()

# Fetch fmtlib
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        12.2.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(fmt)

# Fetch Vulkan Headers
FetchContent_Declare(
    VulkanHeaders
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
    GIT_TAG v1.4.350
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(VulkanHeaders)

# Fetch Volk for Vulkan Loading
if(WIN32)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
endif()

FetchContent_Declare(
    volk
    GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_TAG 1.4.350
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(volk)

# Fetch GLFW
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

# Fetch and configure Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.8-docking
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

# ImGui does not have native support for CMake
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    # ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_compile_definitions(imgui PUBLIC
    VK_NO_PROTOTYPES
    IMGUI_IMPL_VULKAN_USE_VOLK
)

target_link_libraries(imgui PUBLIC glfw Vulkan::Headers volk)

# Fetch and configure ImPlot
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG v1.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(implot)

add_library(implot STATIC
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
    # ${implot_SOURCE_DIR}/implot_demo.cpp
)

target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot PUBLIC imgui)

# Fetch toml++
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(tomlplusplus)

# Add stb_image using subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/stb)

# Executable target
add_executable(fh6-telemetry-server)

target_sources(fh6-telemetry-server
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/ui.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/widgets.cpp
    ${CMAKE_SOURCE_DIR}/src/exec_util.cpp
)

target_include_directories(fh6-telemetry-server PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(fh6-telemetry-server PRIVATE
    asio_standalone
    fmt::fmt
    glfw
    imgui
    implot
    stb_image
    tomlplusplus::tomlplusplus
    volk
    Vulkan::Headers
)

target_compile_definitions(fh6-telemetry-server PRIVATE
    GLFW_INCLUDE_NONE
    IMGUI_IMPL_VULKAN_USE_VOLK
    VK_NO_PROTOTYPES
)

# Sync assets
add_custom_target(sync-assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "${CMAKE_CURRENT_BINARY_DIR}/assets"
    COMMENT "Checking and syncing assets..."
)
add_dependencies(fh6-telemetry-server sync-assets)

# Windows specific: desktop icon and GUI app
if(WIN32)
    target_sources(fh6-telemetry-server PRIVATE
        "${CMAKE_SOURCE_DIR}/platform/windows/app.rc"
    )
endif()

# Linux .desktop file and icon are provided in the platform/linux folder
