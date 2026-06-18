#include <asio.hpp>
#include <cstddef>
#include <format>
#include <iostream>
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
        std::cerr << e.what() << std::endl;
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
        std::cout << std::format(
                         "Speed: {:4.2f} km/h, Gear: {}, Steer: {}",
                         ms_to_kmph(p.Speed), p.Gear, p.Steer
                     )
                  << std::endl;
    else
        std::cout << "Error ocurred!" << std::endl;
}
