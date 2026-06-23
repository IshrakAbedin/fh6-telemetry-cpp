#include <asio.hpp>
#include <fmt/base.h>
#include <stdexcept>
#include <string>
#include <thread>

#include "server/telemetry_packet.hpp"
#include "server/telemetry_server.hpp"
#include "ui/ui.hpp"

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

        ui(server);
        
        io_context.stop();
    }
    catch (std::exception& e)
    {
        fmt::println(stderr, "[Error] {}", e.what());
    }

    return 0;
}
