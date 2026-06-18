#pragma once

#include <asio.hpp>
#include <cstddef>
#include <functional>
#include <memory>

class TelemetryServer
{
  public:
    using udp = asio::ip::udp;
    using port_type = asio::ip::port_type;
    using buffer_t = std::array<char, 1024>;
    using receive_callback_t =
        std::function<void(buffer_t, const std::error_code&, size_t)>;

  private:
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    buffer_t recv_buffer_;
    receive_callback_t recv_callback_;

  public:
    TelemetryServer(
        asio::io_context& io_context, port_type port_num,
        receive_callback_t callback
    )
        : socket_{io_context, udp::endpoint{udp::v4(), port_num}},
          recv_callback_{callback}
    {
        StartReceive();
    }

  private:
    void StartReceive()
    {
        socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            std::bind(
                &TelemetryServer::HandleReceive, this,
                asio::placeholders::error, asio::placeholders::bytes_transferred
            )
        );
    }

    void HandleReceive(
        const std::error_code& error, std::size_t bytes_transferred
    )
    {
        recv_callback_(recv_buffer_, error, bytes_transferred);
        StartReceive();
    }

    void HandleSend(
        std::shared_ptr<std::string> /*message*/,
        const std::error_code& /*error*/, std::size_t /*bytes_transferred*/
    )
    {
    }
};
