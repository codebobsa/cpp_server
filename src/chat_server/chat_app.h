#pragma once

class TcpServer;

class ChatApp
{
public:
	ChatApp();
	~ChatApp() = default;

	void run(const std::string cfgName, const std::string cmd);

private:
	bool init(const std::string& cfgName, const std::string& cmd);
	bool loadConfig(const std::string& cfgName);

private:
	std::unordered_map<std::string, std::shared_ptr<TcpServer>> servers_;
};
