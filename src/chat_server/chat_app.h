#pragma once

class TcpServer;

class ChatApp
{
public:
	ChatApp();
	~ChatApp() = default;

	void run();

private:
	std::unordered_map<std::string, std::shared_ptr<TcpServer>> servers_;
};
