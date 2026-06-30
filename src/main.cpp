#include <asio.hpp>
#include <fmt/base.h>
#include <fmt/os.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <toml++/toml.hpp>

#include "exec_util.hpp"
#include "server/telemetry_packet.hpp"
#include "server/telemetry_server.hpp"
#include "ui/ui.hpp"

constexpr auto DEFAULT_PORT = 7777;
constexpr const char* DEFAULT_CONFIG_CONTENT_TEMPLATE = R"([server]
port = {}
)";
static const auto CONFIG_PATH = (get_executable_dir() / "config.toml").string();

// Read port number from a config file that must be between 0 to
// 65535 and keyed as [server]->[port]-><value>. If the operation
// failes, default port value is returned and a default config file
// is created.
int handle_port_number_from_config(const std::string_view file_path);

int main()
{
    try
    {
        TelemetryServer::port_type PORT =
            handle_port_number_from_config(CONFIG_PATH);

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

int handle_port_number_from_config(const std::string_view file_path)
{
    try
    {
        auto tbl = toml::parse_file(file_path);
        if (auto port_val = tbl["server"]["port"].value<int>())
        {
            int port_num = port_val.value();
            if (port_num < 0 || port_num > 65535)
            {
                throw std::out_of_range(
                    "Port number must be between 0 to 65535"
                );
            }
            return port_num;
        }
        else
        {
            throw std::runtime_error("Properly keyed port value is not found");
        }
    }
    catch (const std::exception& e) // rewrite
    {
        fmt::println(
            "[Warning] Failed to handle config file: \n\t{}", e.what()
        );
        fmt::println("(Re)writing a default config file ...");

        auto config_file = fmt::output_file(CONFIG_PATH.c_str());
        config_file.print(DEFAULT_CONFIG_CONTENT_TEMPLATE, DEFAULT_PORT);
    }

    return DEFAULT_PORT;
}
