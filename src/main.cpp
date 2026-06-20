#include <asio.hpp>
#include <cstddef>
#include <cstdio>
#include <fmt/base.h>
#include <system_error>

#include "telemetry_packet.hpp"
#include "telemetry_server.hpp"
#include "util.hpp"

constexpr auto PORT = 7777;

void PacketPrinter(
    TelemetryServer::buffer_t buffer, const std::error_code& ec,
    size_t bytes_transferred
);

int main()
{
    try
    {
        asio::io_context io_context;
        TelemetryServer server{io_context, PORT, PacketPrinter};
        io_context.run();
    }
    catch (std::exception& e)
    {
        fmt::println(stderr, "[Error] {}", e.what());
    }

    return 0;
}

void PacketPrinter(
    TelemetryServer::buffer_t buffer, const std::error_code& ec,
    size_t bytes_transferred
)
{
    auto p = TelemetryPacket::FromBuffer(buffer);
    if (!ec)
        fmt::println("Speed: {:4.2f} km/h, Gear: {}, Steer: {}",
                         ms_to_kmph(p.Speed), p.Gear, p.Steer);
    else
        fmt::println("Error occured! <{}>", ec.message());
}
