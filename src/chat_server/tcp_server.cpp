#include "precompiled.h"
#include "tcp_session.h"
#include "tcp_server.h"

TcpServer::TcpServer(asio::io_context& ioc, uint16_t port)
	: acceptor_(ioc, tcp::endpoint(tcp::v4(), port))
    , port_(port)
{    
}

void TcpServer::start()
{
    doAccept();
}

void TcpServer::doAccept()
{
    acceptor_.async_accept([this](error_code ec, tcp::socket sock)
        {
            if (!ec)
            {
                spdlog::trace("[TcpServer] New client is accepted {}", sock.native_handle());
                std::make_shared<TcpSession>(std::move(sock))->start();
            }

            doAccept();
        }
    );
}

