#include "stdafx.h"
#include "udp_transfer_server.h"

namespace gpserver {
namespace data_transfer {
namespace udp {

UdpTransferServer::UdpTransferServer(boost::asio::io_service &ios, 
									 unsigned short port, 
									 IPlayerAddrList& player_addr_list)
	: _socket(ios, boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::any(), port)),
	  _player_addr_list(player_addr_list) {
	asyncPeek();
}

void UdpTransferServer::asyncPeek() {
	// 创建一个新的TransferSession
	boost::shared_ptr<TransferSession> session(new TransferSession(_player_addr_list, _socket));
	// 通过message_peek标识异步等待缓冲队列中数据包有到达
	_socket.async_receive_from(boost::asio::null_buffers(),
							   session->src_endpoint(),
							   boost::asio::ip::udp::socket::message_peek,
							   boost::bind(&UdpTransferServer::peekPacketHandler,
										   this,
										   session,
										   boost::asio::placeholders::error,
										   boost::asio::placeholders::bytes_transferred));
}

void UdpTransferServer::peekPacketHandler(boost::shared_ptr<TransferSession> session, 
										  const boost::system::error_code &error, 
										  size_t bytes_transferred) {
	if (!error) {									  
		// 交由TransferSession处理数据
		session->start();	
	
		// 继续接受下一个数据包
		asyncPeek();	
	} else {
		std::clog <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}							  
}

} // namespace udp
} // namespace data_transfer
} // namespace gpserver