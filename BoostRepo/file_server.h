#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <string>

using boost::asio::ip::tcp;

class file_server
{
public:
	file_server(boost::asio::io_service& io_service, unsigned short port, std::string address);
	void start_handle() const;
private:

	mutable tcp::acceptor tcp_acceptor_;

	static void client_session_handler(tcp::socket sock);

	static std::string read_client(tcp::socket& client_sock, unsigned long& bytes_read);
	static unsigned long write_client(tcp::socket& client_sock, const std::string& msg);
	static unsigned long write_client(tcp::socket& client_sock, const std::vector<char>& buf, unsigned long count);
};
