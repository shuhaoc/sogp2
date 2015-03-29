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
	// ����һ���µ�TransferSession
	boost::shared_ptr<TransferSession> session(new TransferSession(_player_addr_list, _socket));
	// ͨ��message_peek��ʶ�첽�ȴ�������������ݰ��е���
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
		// ����TransferSession��������
		session->start();	
	
		// ����������һ�����ݰ�
		asyncPeek();	
	} else {
		std::clog <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}							  
}

} // namespace udp
} // namespace data_transfer
} // namespace gpserver