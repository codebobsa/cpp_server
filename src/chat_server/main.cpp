#include "precompiled.h"
#include "chat_app.h"

int main(int argc, char* argv[])
{
	spdlog::info("{}!", "Hello world");

	ChatApp app;
	app.run();

	return 0;
}