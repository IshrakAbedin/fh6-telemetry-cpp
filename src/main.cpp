#include <asio.hpp>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <fmt/base.h>
#include <stdexcept>
#include <string>
#include <thread>

#include "server/telemetry_packet.hpp"
#include "server/telemetry_server.hpp"
#include "util.hpp"

constexpr auto DEFAULT_PORT = 7777;

int main(int argc, char* argv[])
{
    using namespace std::chrono_literals;
    try
    {
        TelemetryServer::port_type PORT =
            argc < 2 ? DEFAULT_PORT : std::stoi(argv[1]);
        fmt::println("Starting server in port: {} ...", PORT);

        asio::io_context io_context;
        TelemetryServer server{io_context, PORT};
        auto server_thread =
            std::jthread{[&io_context]() { io_context.run(); }};

        while (true)
        {
            if (server.HasNewData())
            {
                auto data = TelemetryPacket::FromBuffer(server.GetData());
                fmt::println(
                    "Speed: {:6.2f}, Gear: {:02d}, RPM: {:6.2f}",
                    ms_to_kmph(data.Speed), data.Gear, data.CurrentEngineRpm
                );
            }
            else
            {
                fmt::println("No new data found");
            }
            std::this_thread::sleep_for(50ms); // Runs for ~20 times per second
        }

        io_context.stop();
    }
    catch (std::exception& e)
    {
        fmt::println(stderr, "[Error] {}", e.what());
    }

    return 0;
}
