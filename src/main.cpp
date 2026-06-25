#include <asio.hpp>
#include <fmt/base.h>
#include <stdexcept>
#include <string>
#include <thread>

#include "server/telemetry_packet.hpp"
#include "server/telemetry_server.hpp"
#include "ui/ui.hpp"

constexpr auto DEFAULT_PORT = 7777;

// Return parsed port number between 0 to 65535 if valid,
// -1 if out of range and -2 if argument is not an int
int get_port_number(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int port_num = get_port_number(argc, argv);

    if (port_num == -1)
    {
        fmt::println(stderr, "[Error] Port number must be between 0 to 65535");
        return 1;
    }
    else if (port_num == -2)
    {
        fmt::println(
            stderr, "[Error] Provide a valid port number between 0 to 65535 as "
                    "the first argument"
        );
        return 2;
    }

    try
    {
        TelemetryServer::port_type PORT = port_num;

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

int get_port_number(int argc, char* argv[])
{
    int port_num;
    if (argc < 2)
    {
        port_num = DEFAULT_PORT;
    }
    else
    {
        try
        {
            port_num = std::stoi(argv[1]);
            if (port_num < 0 || port_num > 65535)
            {
                return -1;
            }
        }
        catch (const std::exception& e)
        {
            return -2;
        }
    }
    return port_num;
}
