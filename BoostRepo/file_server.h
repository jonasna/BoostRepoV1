#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <string>

namespace asio = boost::asio::ip;

class file_server
{
public:
	file_server(boost::asio::io_service& io_service, unsigned short port, std::string address);
	void listen() const;

private:

	mutable asio::tcp::acceptor tcp_acceptor_;

	static void client_session_handler(asio::tcp::socket sock);

	static std::string read_client(asio::tcp::socket& client_sock, unsigned long& bytes_read);

	static unsigned long write_client(asio::tcp::socket& client_sock, const std::string& msg);
	static unsigned long write_client(asio::tcp::socket& client_sock, const std::vector<char>& buf);
	static unsigned long write_client(asio::tcp::socket& client_sock, const char* buf, unsigned long count);
};
