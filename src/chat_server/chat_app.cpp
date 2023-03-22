#include "precompiled.h"
#include "tcp_server.h"
#include "chat_app.h"

ChatApp::ChatApp()
{

}

void ChatApp::run()
{
	asio::io_context ioc;

	std::shared_ptr<TcpServer> server = std::make_shared<TcpServer>(ioc, 3037);

	server->start();

	servers_.emplace("main_chat", server);
}