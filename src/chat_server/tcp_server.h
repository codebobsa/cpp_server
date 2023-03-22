#pragma once

class TcpServer
{
public:
    TcpServer(asio::io_context& ioc, uint16_t port);

	void start();

private:
	void doAccept();

private:
	tcp::acceptor acceptor_;
	uint16_t port_;
};

