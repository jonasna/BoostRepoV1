#include "file_server.h"
#include "f_section_reader.h"
#include <thread>
#include <iostream>


using namespace boost::asio;
using ip::tcp;

file_server::file_server(io_service & io_service, unsigned short port, std::string address)
	: tcp_acceptor_(io_service, tcp::endpoint(ip::address_v4::from_string(address), port))
{
}

void file_server::listen() const
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
		unsigned long count;
		auto file_name = read_client(sock, count);

		f_section_reader file_handle(file_name, 1000);

		auto file_size = file_handle.get_file_size();

		write_client(sock, std::to_string(file_size));

		if(file_handle.get_is_valid())
		{
			while(file_handle.get_remaining_bytes())
			{
				write_client(sock, file_handle.get_next_section());
			}
		}

		sock.shutdown(sock.shutdown_both);
		
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
	return write_client(client_sock, std::string(msg + '\0').c_str(), msg.length() + 1);
}

unsigned long file_server::write_client(tcp::socket& client_sock, const std::vector<char>& buf)
{
	return write_client(client_sock, buf.data(), buf.size());
}

unsigned long file_server::write_client(tcp::socket& client_sock, const char* buf, unsigned long count)
{
	const const_buffer write_buf = buffer(buf, count);
	return static_cast<unsigned long>(write(client_sock, write_buf));
}
