#include "stdafx.h"
#include "simple_room.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace gpserver {
namespace players {

SimpleRoom::SimpleRoom(boost::asio::io_service &ios, unsigned short port)
	: _ios(ios),
	  _acceptor(ios, tcp::endpoint(address_v4::any(), port)) {
	asyncAccept();	
}	 

void SimpleRoom::asyncAccept() {
	boost::shared_ptr<tcp::socket> socket(new tcp::socket(_ios));
	_acceptor.async_accept(
		*socket,
		boost::bind(&SimpleRoom::acceptHanlder,
					this,
					socket,
					placeholders::error));
} 

void SimpleRoom::acceptHanlder(boost::shared_ptr<tcp::socket> socket, 
							   const boost::system::error_code &error) {
	if (!error) {
		//std::cout << socket->remote_endpoint().address().to_string() << " entered" << std::endl;
		SocketList::iterator last_itr = _client_list.insert(_client_list.end(), socket);
		socket->async_receive(
			null_buffers(),
			boost::bind(&SimpleRoom::receiveHandler,
						this,
						last_itr,
						placeholders::error,
						placeholders::bytes_transferred));	
		asyncAccept();												  
	} else {
		std::clog <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}
}

void SimpleRoom::receiveHandler(SimpleRoom::SocketList::iterator socket_iterator, 
								const boost::system::error_code &error, 
								size_t bytes_transferred) {
	_client_list.erase(socket_iterator);								
}

std::list<unsigned long> SimpleRoom::get(unsigned long src_ip) const {
	std::list<unsigned long> ret;
	for (SocketList::const_iterator itr = _client_list.begin(); itr != _client_list.end(); itr++) {
		ret.push_back((*itr)->remote_endpoint().address().to_v4().to_ulong());
	}
	return ret;
}

} // namespace players
} // namespace gpserver