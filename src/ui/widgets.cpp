#include "ui/widgets.hpp"

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "server/telemetry_packet.hpp"
#include "ui/ui_config.hpp"
#include "ui/ui_util.hpp"
#include "util.hpp"

static const ImColor ForzaBlue = ImColor{52, 76, 199, 255};
static const ImColor ForzaPink = ImColor{212, 38, 151, 255};
static const ImColor ForzaYellow = ImColor{245, 226, 59, 255};
static const ImColor ForzaGreen = ImColor{36, 128, 54, 255};
static const ImColor ForzaViolet = ImColor{108, 47, 148, 255};

void general_info_plot(
    float speed, int gear, int car_class, int drivetrain, bool receiving
);
void powertrain_plot(float currentRPM, float maxRPM, float torque, float power);
void input_plot(float acc, float brk, float hbk, float clt, float str);

void widgets(TelemetryServer& server)
{

    auto data = TelemetryPacket::FromBuffer(server.GetData());
    bool has_data = true;

    has_data = server.HasNewData();

    general_info_plot(
        data.Speed, data.Gear, data.CarClass, data.DrivetrainType, has_data
    );
    powertrain_plot(
        data.CurrentEngineRpm, data.EngineMaxRpm, data.Torque, data.Power
    );
    input_plot(
        data.Accel / 255.0f, data.Break / 255.0f, data.HandBrake / 255.0f,
        data.Clutch / 255.0f, clamp((-1.0f * data.Steer) / 127.0f, -1.0f, 1.0f)
    );
}

void general_info_plot(
    float speed, int gear, int car_class, int drivetrain, bool receiving
)
{
    ImGui::Begin("General Info");

    const auto& current_style = ImGui::GetStyle();

    ImGui::PushFont(
        nullptr, current_style.FontSizeBase * GENERAL_INFO_FONT_SCALE
    );

    ImGui::Text(
        "Speed: %03.0f kmph (%03.0f mph)", ms_to_kmph(speed), ms_to_mph(speed)
    );
    ImGui::ProgressBar(clamp(ms_to_kmph(speed) / 500.0f, 0.0f, 1.0f));
    ImGui::Text("Gear : %02d", gear);
    ImGui::Text("Class: %s", GetCarClassString(car_class));
    ImGui::Text("Drivetrain: %s", GetDrivetrainString(drivetrain));

    ImColor status_color =
        receiving ? ImColor{0, 255, 0, 255} : ImColor{255, 0, 0, 255};
    ImGui::ColorButton(
        "##Status", status_color, ImGuiColorEditFlags_NoTooltip, ImVec2(16, 16)
    );

    ImGui::PopFont();

    ImGui::End();
}

void powertrain_plot(float currentRPM, float maxRPM, float torque, float power)
{
    ImGui::Begin("Powertrain");

    // static ScrollingBuffer sdata1, sdata2;

    static ScrollingBuffer transmission_data, torque_data, power_data;

    // Add points to the buffers every 0.02 seconds
    static float t = 0, last_t = 0.0f;

    if (t == 0 || t - last_t >= 0.02f)
    {
        transmission_data.AddPoint(t, currentRPM);
        torque_data.AddPoint(t, torque);
        power_data.AddPoint(t, power);
        last_t = t;
    }
    t += ImGui::GetIO().DeltaTime;

    static float history = 10.0f;
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

    // static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

    if (ImPlot::BeginPlot(
            "##Powertrain",
            ImVec2(-1, -1) //, ImVec2(-1, ImGui::GetTextLineHeight() * 10)
        ))
    {
        // General X and Rev Y Axis
        ImPlot::SetupAxes("Time (s)", "RPM"); //, flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, maxRPM, ImGuiCond_Always);

        // Torque/Power Y Axis
        ImPlot::SetupAxis(
            ImAxis_Y2, "Torque (N-m) | Power (KW)", ImPlotAxisFlags_AuxDefault
        );
        ImPlot::SetupAxisLimits(ImAxis_Y2, 0.0, 1400.0);

        // Plot Rev data in the default axis
        static ImPlotSpec transmission_spec;
        transmission_spec.Offset = transmission_data.Offset;
        transmission_spec.Stride = 2 * sizeof(float);
        transmission_spec.LineColor = ForzaBlue;
        transmission_spec.FillColor = transmission_spec.LineColor;
        transmission_spec.FillAlpha = 0.5f;
        ImPlot::PlotShaded(
            "Transmission", &transmission_data.Data[0].x,
            &transmission_data.Data[0].y, transmission_data.Data.size(),
            -INFINITY, transmission_spec
        );
        ImPlot::PlotLine(
            "Transmission", &transmission_data.Data[0].x,
            &transmission_data.Data[0].y, transmission_data.Data.size(),
            transmission_spec
        );

        // Switch to the secondary axis and plot torque and power
        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);

        static ImPlotSpec torque_spec;
        torque_spec.Offset = torque_data.Offset;
        torque_spec.Stride = 2 * sizeof(float);
        torque_spec.LineColor = ForzaPink;
        ImPlot::PlotLine(
            "Torque", &torque_data.Data[0].x, &torque_data.Data[0].y,
            torque_data.Data.size(), torque_spec
        );

        static ImPlotSpec power_spec;
        power_spec.Offset = power_data.Offset;
        power_spec.Stride = 2 * sizeof(float);
        power_spec.LineColor = ForzaYellow;
        ImPlot::PlotLine(
            "Power", &power_data.Data[0].x, &power_data.Data[0].y,
            power_data.Data.size(), power_spec
        );

        ImPlot::EndPlot();
    }

    ImGui::End();
}

void input_plot(float acc, float brk, float hbk, float clt, float str)
{
    ImGui::Begin("Inputs");

    static ScrollingBuffer acc_data, brk_data, hbk_data, clt_data, str_data;

    static float t = 0, last_t = 0.0f;

    if (t == 0 || t - last_t >= 0.02f)
    {
        acc_data.AddPoint(t, acc);
        brk_data.AddPoint(t, brk);
        hbk_data.AddPoint(t, hbk);
        clt_data.AddPoint(t, clt);
        str_data.AddPoint(t, str);
        last_t = t;
    }
    t += ImGui::GetIO().DeltaTime;

    static float history = 10.0f;
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

    if (ImPlot::BeginPlot("##Inputs", ImVec2(-1, -1)))
    {
        // Default axis for all inputs except steering
        ImPlot::SetupAxes("Time (s)", "Actuation Factor");
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1.0f, ImGuiCond_Always);

        // Steering Y axis
        ImPlot::SetupAxis(
            ImAxis_Y2, "Steering (1 = left, -1 = right)",
            ImPlotAxisFlags_AuxDefault
        );
        ImPlot::SetupAxisLimits(ImAxis_Y2, -1.0, 1.0);

        static ImPlotSpec acc_spec;
        acc_spec.Offset = acc_data.Offset;
        acc_spec.Stride = 2 * sizeof(float);
        acc_spec.LineColor = ForzaBlue;
        acc_spec.FillColor = acc_spec.LineColor;
        acc_spec.FillAlpha = 0.4f;
        ImPlot::PlotShaded(
            "Accelerator", &acc_data.Data[0].x, &acc_data.Data[0].y,
            acc_data.Data.size(), -INFINITY, acc_spec
        );
        ImPlot::PlotLine(
            "Accelerator", &acc_data.Data[0].x, &acc_data.Data[0].y,
            acc_data.Data.size(), acc_spec
        );

        static ImPlotSpec brk_spec;
        brk_spec.Offset = brk_data.Offset;
        brk_spec.Stride = 2 * sizeof(float);
        brk_spec.LineColor = ForzaPink;
        brk_spec.FillColor = brk_spec.LineColor;
        brk_spec.FillAlpha = 0.4f;
        ImPlot::PlotShaded(
            "Brake", &brk_data.Data[0].x, &brk_data.Data[0].y,
            brk_data.Data.size(), -INFINITY, brk_spec
        );
        ImPlot::PlotLine(
            "Brake", &brk_data.Data[0].x, &brk_data.Data[0].y,
            brk_data.Data.size(), brk_spec
        );

        static ImPlotSpec hbk_spec;
        hbk_spec.Offset = hbk_data.Offset;
        hbk_spec.LineColor = ForzaYellow;
        hbk_spec.Stride = 2 * sizeof(float);
        ImPlot::PlotLine(
            "Hand-brake", &hbk_data.Data[0].x, &hbk_data.Data[0].y,
            hbk_data.Data.size(), hbk_spec
        );

        static ImPlotSpec clt_spec;
        clt_spec.Offset = clt_data.Offset;
        clt_spec.LineColor = ForzaViolet;
        clt_spec.Stride = 2 * sizeof(float);
        ImPlot::PlotLine(
            "Cluth", &clt_data.Data[0].x, &clt_data.Data[0].y,
            clt_data.Data.size(), clt_spec
        );

        // Switch to the second Y axis and plot steering data
        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);

        static ImPlotSpec steering_spec;
        steering_spec.Offset = steering_spec.Offset;
        steering_spec.Stride = 2 * sizeof(float);
        steering_spec.LineColor = ForzaGreen;
        ImPlot::PlotLine(
            "Steering", &str_data.Data[0].x, &str_data.Data[0].y,
            str_data.Data.size(), steering_spec
        );

        ImPlot::EndPlot();
    }
    ImGui::End();
}
