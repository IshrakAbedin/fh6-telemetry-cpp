#include "ui/widgets.hpp"

#include <fmt/format.h>
#include <imgui.h>

#include "server/telemetry_packet.hpp"
#include "util.hpp"

void widgets(TelemetryServer& server)
{
    ImGui::Begin("Test Window");

    auto data = TelemetryPacket::FromBuffer(server.GetData());
    ImGui::Text("Speed: %6.2f", ms_to_kmph(data.Speed));
    ImGui::Text("Gear : %02d", data.Gear);

    float rpm_fraction = data.CurrentEngineRpm / data.EngineMaxRpm;
    auto rpm_progress_text =
        fmt::format("{:.0f}/{:.0f}", data.CurrentEngineRpm, data.EngineMaxRpm);
    ImGui::ProgressBar(
        rpm_fraction, ImVec2{0.0f, 0.0f}, rpm_progress_text.c_str()
    );
    ImGui::Text("Speed: %6.2f", ms_to_kmph(data.Speed));

    if (server.HasNewData())
    {
        ImGui::TextColored(ImColor{0, 200, 0}, "------");
    }
    else
    {
        ImGui::TextColored(ImColor{200, 0, 0}, "XXXXXX");
    }

    ImGui::End();
}
