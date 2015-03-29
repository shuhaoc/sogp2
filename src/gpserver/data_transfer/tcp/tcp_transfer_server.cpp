#include "stdafx.h"
#include "tcp_transfer_server.h"

namespace gpserver {
namespace data_transfer {
namespace tcp {

TcpTransferServer::TcpTransferServer(boost::asio::io_service& ios, unsigned short port) 
	: _ios(ios),
	  _acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port)) {
	asyncAccept();
}

void TcpTransferServer::asyncAccept() {
	// ����һ�����ӽ׶Σ���һ�׶Σ��ĻỰ
	boost::shared_ptr<ConnectStageSession> session(new ConnectStageSession(_ios));
	_acceptor.async_accept(
		session->src_socket(),
		boost::bind(&TcpTransferServer::acceptHandler, 
					this, 
					session, 
					boost::asio::placeholders::error));
}

void TcpTransferServer::acceptHandler(boost::shared_ptr<ConnectStageSession> session,
									  const boost::system::error_code& error) {
	if (!error) {
		// �����Ự�����ConnectStageSession����
		session->start();		
		// �����첽��������
		asyncAccept();
	} else {
		std::clog <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}
}

} // namespace tcp
} // namespace data_transfer
} // namespace gpserver