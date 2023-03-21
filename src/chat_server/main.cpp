#include <iostream>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[])
{
	spdlog::info("{}!", "Hello world");

	return 0;
}