#include "precompiled.h"
#include "chat_app.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Usage: chat_server cfgname [start | stop]" << std::endl;
		return 1;
	}

	ChatApp app;
	app.run(argv[1], argv[2]);

	return 0;
}