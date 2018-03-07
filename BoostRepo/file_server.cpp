#include "file_server.h"
#include <thread>
#include <iostream>

using namespace boost::asio;


file_server::file_server(io_service & io_service, unsigned short port, std::string address)
	: tcp_acceptor_(io_service, tcp::endpoint(ip::address_v4::from_string(address), port))
{
}

void file_server::start_handle() const
{
	for(;;)
	{
		std::thread(client_session_handler, tcp_acceptor_.accept()).detach();
	}
}

void file_server::client_session_handler(tcp::socket sock)
{
	try
	{
		std::vector<char> lol{ 'H', 'e', 'j', '\n' };
		write_client(sock, lol, 4);
		unsigned long count;
		std::cout << read_client(sock, count) << std::endl;

	}catch(std::exception& e)
	{
		std::cerr << "Error in thread: " << e.what() << std::endl;
	}
}

std::string file_server::read_client(tcp::socket& client_sock, unsigned long& bytes_read)
{
	streambuf read_buffer;
	bytes_read = static_cast<unsigned long>(read_until(client_sock, read_buffer, '\0'));
	return {
		buffers_begin(read_buffer.data()),
		buffers_end(read_buffer.data()) - 1
	};
}

unsigned long file_server::write_client(tcp::socket& client_sock, const std::string& msg)
{
	streambuf write_buffer;
	std::ostream output(&write_buffer);
	output << msg + '\n';
	return static_cast<unsigned long>(write(client_sock, write_buffer));
}


unsigned long file_server::write_client(tcp::socket& client_sock, const std::vector<char>& buf, unsigned long count)
{
	streambuf write_buffer;
	std::ostream output(&write_buffer);
	output.write(buf.data(), count);
	return static_cast<unsigned long>(write(client_sock, write_buffer));
}
