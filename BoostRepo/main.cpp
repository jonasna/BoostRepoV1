#include "boost/asio.hpp"
#include "file_server.h"
#include <iostream>

int main()
{
	boost::asio::io_service io_service;
	try
	{
		file_server server(io_service, 9000, "192.168.1.236");
		server.start_handle();
	}catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
