#include "precompiled.h"
#include <boost/property_tree/ini_parser.hpp>
#include "tcp_server.h"
#include "chat_app.h"

ChatApp::ChatApp()
{

}

bool ChatApp::init(const std::string& cfgName, const std::string& cmd)
{
	if (loadConfig(cfgName) == false)
	{
		return false;
	}

	return true;
}

bool ChatApp::loadConfig(const std::string& cfgName)
{
	namespace property_tree = boost::property_tree;

	std::string cfgPath = fmt::format("./cfg/{}.cfg", cfgName);
	
	try
	{
		property_tree::ptree reader;
		property_tree::ini_parser::read_ini(cfgPath, reader);

		uint16_t port = reader.get<uint16_t>("server.port");
	}
	catch (std::exception& e)
	{
		spdlog::error("exception: {}", e.what());
		return false;
	}

	return true;
}

void ChatApp::run(const std::string cfgName, const std::string cmd)
{
	if (init(cfgName, cmd) == false)
	{
		spdlog::error("[ChatApp] cfg parsing error");
		return;
	}

	asio::io_context ioc;

	std::shared_ptr<TcpServer> server = std::make_shared<TcpServer>(ioc, 3037);

	server->start();

	servers_.emplace("chat_server", server);

	spdlog::info("server started at {} port", 3037);

	ioc.run();
}