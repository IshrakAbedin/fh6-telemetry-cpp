#pragma once

#include <asio.hpp>
#include <cstddef>
#include <functional>
#include <memory>

#include "server/triple_buffer.hpp"

class TelemetryServer
{
  public:
    using udp = asio::ip::udp;
    using port_type = asio::ip::port_type;
    using buffer_t = std::array<char, 1024>;
    using receive_callback_t =
        std::function<void(buffer_t, const std::error_code&, size_t)>;

  private:
    udp::socket m_Socket;
    udp::endpoint m_RemoteEndpoint;
    TripleBuffer<buffer_t> m_RecvTBuffer;

  public:
    TelemetryServer(asio::io_context& io_context, port_type port_num)
        : m_Socket{io_context, udp::endpoint{udp::v4(), port_num}}
    {
        StartReceive();
    }

    bool HasNewData()
    {
        return m_RecvTBuffer.CheckNew();
    }

    const buffer_t& GetData() const
    {
        return m_RecvTBuffer.GetReadBuffer();
    }

  private:
    void StartReceive()
    {
        m_Socket.async_receive_from(
            asio::buffer(m_RecvTBuffer.GetWriteBuffer()), m_RemoteEndpoint,
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
        // Forza specified packet size (324)
        if (!error && bytes_transferred <= 324)
            m_RecvTBuffer.Publish();
        StartReceive();
    }
};
