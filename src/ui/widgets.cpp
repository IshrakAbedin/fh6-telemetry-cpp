#include "ui/widgets.hpp"

#include <fmt/format.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>

#include "data_util.hpp"
#include "server/telemetry_packet.hpp"
#include "ui/ui_config.hpp"
#include "ui/ui_theme.hpp"
#include "ui/ui_util.hpp"

void general_info_plot(
    float speed, int gear, float boost, int car_class, int drivetrain,
    bool receiving
);
void powertrain_plot(float currentRPM, float maxRPM, float torque, float power);
void input_plot(float acc, float brk, float hbk, float clt, float str);
void gg_plot(float right_g, float forward_g);
void suspension_plot(float fl, float fr, float rl, float rr);
void tiretemp_plot(float fl, float fr, float rl, float rr);
void tireslip_plot(float fl, float fr, float rl, float rr);

void widgets(TelemetryServer& server)
{

    auto data = TelemetryPacket::FromBuffer(server.GetData());
    bool has_data = true;

    has_data = server.HasNewData();

    general_info_plot(
        data.Speed, data.Gear, data.Boost, data.CarClass, data.DrivetrainType,
        has_data
    );
    powertrain_plot(
        data.CurrentEngineRpm, data.EngineMaxRpm, data.Torque, data.Power
    );
    input_plot(
        data.Accel / 255.0f, data.Break / 255.0f, data.HandBrake / 255.0f,
        data.Clutch / 255.0f, clamp((-1.0f * data.Steer) / 127.0f, -1.0f, 1.0f)
    );
    gg_plot(accel_to_g(data.AccelerationX), accel_to_g(data.AccelerationZ));
    suspension_plot(
        data.NormalizedSuspensionTravelFrontLeft,
        data.NormalizedSuspensionTravelFrontRight,
        data.NormalizedSuspensionTravelRearLeft,
        data.NormalizedSuspensionTravelRearRight
    );
    tiretemp_plot(
        data.TireTempFrontLeft, data.TireTempFrontRight, data.TireTempRearLeft,
        data.TireTempRearRight
    );
    tireslip_plot(
        data.TireSlipRatioFrontLeft, data.TireSlipRatioFrontRight,
        data.TireSlipRatioRearLeft, data.TireSlipRatioRearRight
    );
}

void general_info_plot(
    float speed, int gear, float boost, int car_class, int drivetrain,
    bool receiving
)
{
    ImGui::Begin("General Info");

    const auto& current_style = ImGui::GetStyle();

    ImGui::PushFont(
        nullptr, current_style.FontSizeBase * GENERAL_INFO_FONT_SCALE
    );
    ImGui::Text(
        "Class: %s - %s", GetCarClassString(car_class),
        GetDrivetrainString(drivetrain)
    );
    ImGui::SameLine();
    ImColor status_color =
        receiving ? ImColor{0, 255, 0, 255} : ImColor{255, 0, 0, 255};
    ImGui::ColorButton(
        "##Status", status_color, ImGuiColorEditFlags_NoTooltip,
        ImVec2{16.0f, 16.0f}
    );
    ImGui::Separator();

    ImGui::Text(
        "Speed: %03.0f kmph (%03.0f mph)", ms_to_kmph(speed), ms_to_mph(speed)
    );
    std::string speed_text = fmt::format("{:.1f} kmph", ms_to_kmph(speed));

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ColorPink.Value);
    ImGui::ProgressBar(
        clamp(ms_to_kmph(speed) / 500.0f, 0.0f, 1.0f), ImVec2{-1.0f, 0.0f},
        speed_text.c_str()
    );
    ImGui::PopStyleColor();

    ImGui::Separator();

    ImGui::Text("Gear : %02d", gear);
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
    ImGui::Text("Boost: %4.2f bar", psi_to_bar(boost));

    ImGui::PopFont();

    ImGui::End();
}

void powertrain_plot(float currentRPM, float maxRPM, float torque, float power)
{
    ImGui::Begin("Powertrain");

    static ScrollingBuffer rpm_data, torque_data, power_data;

    // Add points to the buffers every 0.02 seconds (rate limited)
    // static float t = 0, last_t = 0.0f;
    // if (t == 0 || t - last_t >= 0.02f)
    // {
    //     rpm_data.AddPoint(t, currentRPM);
    //     torque_data.AddPoint(t, torque);
    //     power_data.AddPoint(t, power);
    //     last_t = t;
    // }
    // t += ImGui::GetIO().DeltaTime;

    // Plot without rate limiting
    static float t = 0;
    rpm_data.AddPoint(t, currentRPM);
    torque_data.AddPoint(t, torque);
    power_data.AddPoint(t, power / 1000.0f); // Convert watts to kilo watts
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
        static ImPlotSpec rpm_spec;
        rpm_spec.Offset = rpm_data.Offset;
        rpm_spec.Stride = 2 * sizeof(float);
        rpm_spec.LineWeight = 2.0f;
        rpm_spec.LineColor = ColorBlue;
        rpm_spec.FillColor = rpm_spec.LineColor;
        rpm_spec.FillAlpha = 0.35f;
        ImPlot::PlotShaded(
            "Engine RPM", &rpm_data.Data[0].x, &rpm_data.Data[0].y,
            rpm_data.Data.size(), -INFINITY, rpm_spec
        );
        ImPlot::PlotLine(
            "Engine RPM", &rpm_data.Data[0].x, &rpm_data.Data[0].y,
            rpm_data.Data.size(), rpm_spec
        );

        // Switch to the secondary axis and plot torque and power
        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);

        static ImPlotSpec torque_spec;
        torque_spec.Offset = torque_data.Offset;
        torque_spec.Stride = 2 * sizeof(float);
        torque_spec.LineWeight = 2.0f;
        torque_spec.LineColor = ColorPink;
        ImPlot::PlotLine(
            "Torque", &torque_data.Data[0].x, &torque_data.Data[0].y,
            torque_data.Data.size(), torque_spec
        );

        static ImPlotSpec power_spec;
        power_spec.Offset = power_data.Offset;
        power_spec.Stride = 2 * sizeof(float);
        power_spec.LineWeight = 2.0f;
        power_spec.LineColor = ColorYellow;
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

    // Add points to the buffers every 0.02 seconds (rate limited)
    // static float t = 0, last_t = 0.0f;
    // if (t == 0 || t - last_t >= 0.02f)
    // {
    //     acc_data.AddPoint(t, acc);
    //     brk_data.AddPoint(t, brk);
    //     hbk_data.AddPoint(t, hbk);
    //     clt_data.AddPoint(t, clt);
    //     str_data.AddPoint(t, str);
    //     last_t = t;
    // }
    // t += ImGui::GetIO().DeltaTime;

    // Plot without rate limiting
    static float t = 0;
    acc_data.AddPoint(t, acc);
    brk_data.AddPoint(t, brk);
    hbk_data.AddPoint(t, hbk);
    clt_data.AddPoint(t, clt);
    str_data.AddPoint(t, str);
    t += ImGui::GetIO().DeltaTime;

    static float history = 10.0f;
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

    if (ImPlot::BeginPlot("##Inputs", ImVec2(-1, -1)))
    {
        // Default axis for all inputs except steering
        ImPlot::SetupAxes("Time (s)", "Actuation Factor");
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1.05, ImGuiCond_Always);

        // Steering Y axis
        ImPlot::SetupAxis(
            ImAxis_Y2, "Steering (-1 = Left, +1 = Right)",
            ImPlotAxisFlags_AuxDefault
        );
        ImPlot::SetupAxisLimits(ImAxis_Y2, -1.1, 1.1);

        static ImPlotSpec acc_spec;
        acc_spec.Offset = acc_data.Offset;
        acc_spec.Stride = 2 * sizeof(float);
        acc_spec.LineWeight = 2.0f;
        acc_spec.LineColor = ColorBlue;
        acc_spec.FillColor = acc_spec.LineColor;
        acc_spec.FillAlpha = 0.35f;
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
        brk_spec.LineWeight = 2.0f;
        brk_spec.LineColor = ColorPink;
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
        hbk_spec.LineWeight = 2.0f;
        hbk_spec.LineColor = ColorYellow;
        hbk_spec.Stride = 2 * sizeof(float);
        ImPlot::PlotLine(
            "Hand-brake", &hbk_data.Data[0].x, &hbk_data.Data[0].y,
            hbk_data.Data.size(), hbk_spec
        );

        static ImPlotSpec clt_spec;
        clt_spec.Offset = clt_data.Offset;
        clt_spec.LineWeight = 2.0f;
        clt_spec.LineColor = ColorGreen;
        clt_spec.Stride = 2 * sizeof(float);
        ImPlot::PlotLine(
            "Clutch", &clt_data.Data[0].x, &clt_data.Data[0].y,
            clt_data.Data.size(), clt_spec
        );

        // Switch to the second Y axis and plot steering data
        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);

        static ImPlotSpec steering_spec;
        steering_spec.Offset = str_data.Offset;
        steering_spec.LineWeight = 2.0f;
        steering_spec.Stride = 2 * sizeof(float);
        steering_spec.LineColor = ColorViolet;
        ImPlot::PlotLine(
            "Steering", &str_data.Data[0].x, &str_data.Data[0].y,
            str_data.Data.size(), steering_spec
        );

        ImPlot::EndPlot();
    }
    ImGui::End();
}

void gg_plot(float right_g, float forward_g)
{
    // Invert axes
    right_g *= -1;
    forward_g *= -1;

    static ImPlotSpec gg_spec{ImPlotProp_Marker,     ImPlotMarker_Square,
                              ImPlotProp_MarkerSize, 12,
                              ImPlotProp_LineWeight, 2.0f,
                              ImPlotProp_LineColor,  ColorViolet,
                              ImPlotProp_FillColor,  ColorViolet,
                              ImPlotProp_FillAlpha,  0.25f};

    ImGui::Begin("G-Forces");
    if (ImPlot::BeginPlot("##GG", ImVec2(-1, -1)))
    {
        ImPlot::SetupAxes(nullptr, nullptr);
        ImPlot::SetupAxisLimits(ImAxis_X1, -3.0, 3.0, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, -3.0, 3.0, ImGuiCond_Always);
        ImPlot::PlotScatter("", &right_g, &forward_g, 1, gg_spec);

        ImPlot::EndPlot();
    }
    ImGui::End();
}

void suspension_plot(float fl, float fr, float rl, float rr)
{
    ImGui::Begin("Suspension");

    float region_width = ImGui::GetContentRegionAvail().x;
    float item_gap_width = ImGui::GetStyle().ItemSpacing.x;
    float progressbar_width = (region_width - item_gap_width) / 2.0f;

    ImGui::ProgressBar(fl, ImVec2{progressbar_width, 0});
    ImGui::SameLine();
    ImGui::ProgressBar(fr, ImVec2{progressbar_width, 0});

    ImGui::ProgressBar(rl, ImVec2{progressbar_width, 0});
    ImGui::SameLine();
    ImGui::ProgressBar(rr, ImVec2{progressbar_width, 0});

    ImGui::End();
}

void tiretemp_plot(float fl, float fr, float rl, float rr)
{
    ImGui::Begin("Tire Temperatures");
    ImGui::BeginTable("tire_temp_table", 2, ImGuiTableFlags_Borders);

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text(
        "%.2f\xc2\xb0"
        "C",
        fahrenheit_to_celcius(fl)
    );
    ImGui::TableNextColumn();
    ImGui::Text(
        "%.2f\xc2\xb0"
        "C",
        fahrenheit_to_celcius(fr)
    );
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text(
        "%.2f\xc2\xb0"
        "C",
        fahrenheit_to_celcius(rl)
    );
    ImGui::TableNextColumn();
    ImGui::Text(
        "%.2f\xc2\xb0"
        "C",
        fahrenheit_to_celcius(rr)
    );

    ImGui::EndTable();
    ImGui::End();
}

void tireslip_plot(float fl, float fr, float rl, float rr)
{
    auto warnable_imgui_text = [](float f)
    {
        if (f < -1.0f || f > 1.0f)
        {
            ImGui::TextColored(ColorYellow, "%.3f", f);
        }
        else
        {
            ImGui::Text("%.3f", f);
        }
    };

    ImGui::Begin("Tire Slip Ratios");
    ImGui::BeginTable("tire_slip_table", 2, ImGuiTableFlags_Borders);

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    warnable_imgui_text(fl);
    ImGui::TableNextColumn();
    warnable_imgui_text(fr);
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    warnable_imgui_text(rl);
    ImGui::TableNextColumn();
    warnable_imgui_text(rr);

    ImGui::EndTable();
    ImGui::End();
}
