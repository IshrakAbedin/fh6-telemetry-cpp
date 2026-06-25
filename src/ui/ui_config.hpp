#pragma once

#include "exec_util.hpp"

constexpr auto WINDOW_TITLE = "Forza Horizon 6 Telemetry";
constexpr auto WINDOW_WIDTH = 1280;
constexpr auto WINDOW_HEIGHT = 720;
constexpr auto GENERAL_INFO_FONT_SCALE = 1.25f;

inline std::string GET_FONT_PATH()
{
    static auto font_path = (get_executable_dir() /
                             "assets/font/electrolize/Electrolize-Regular.ttf")
                                .string();
    return font_path;
}

inline std::string GET_IMGUI_INI_PATH()
{
    static auto ini_path = (get_executable_dir() / "imgui.ini").string();
    return ini_path;
}
