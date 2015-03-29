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
	// 创建一个连接阶段（第一阶段）的会话
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
		// 启动会话，详见ConnectStageSession定义
		session->start();		
		// 继续异步接受连接
		asyncAccept();
	} else {
		std::clog <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}
}

} // namespace tcp
} // namespace data_transfer
} // namespace gpserver