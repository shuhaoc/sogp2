#include "stdafx.h"
#include "connect_stage_session.h"
#include "transfer_stage_session.h"

namespace gpserver {
namespace data_transfer {
namespace tcp {

ConnectStageSession::ConnectStageSession(boost::asio::io_service& ios)
	: _src_socket(new boost::asio::ip::tcp::socket(ios)),
	  _dest_socket(new boost::asio::ip::tcp::socket(ios)) {
}

void ConnectStageSession::start() {
	//std::cout << __FUNCTION__ << std::endl;
	// 异步读取目标地址、端口信息
	boost::asio::async_read(
		*_src_socket,
		boost::asio::buffer(&_dest_endpoint, sizeof(_dest_endpoint)), 
		boost::bind(&ConnectStageSession::recvDestEndpointHandler, 
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));			
}

void ConnectStageSession::recvDestEndpointHandler(const boost::system::error_code& error,
												  size_t bytes_transferred) {
	if (!error && bytes_transferred == sizeof(_dest_endpoint)) {
		// 读取成功，异步连接之
		_dest_socket->async_connect(
			boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4(ntohl(_dest_endpoint.ip32)), 
										   ntohs(_dest_endpoint.port)),
			boost::bind(&ConnectStageSession::connectDestinationHandler,
						shared_from_this(),
						boost::asio::placeholders::error));
	} else {
		std::clog <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}
}

void ConnectStageSession::connectDestinationHandler(const boost::system::error_code& error) {
	// 异步返回连接结果，如果有什么错误已经发生，可以拖延至第三步中处理，
	// 因为ConnectResult总是要尝试返回给用户的
	_connect_result.success = !error;
	boost::asio::async_write(
		*_src_socket,
		boost::asio::buffer(&_connect_result, sizeof(_connect_result)),
		boost::bind(&ConnectStageSession::sendConnectResultHandler,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));	
	if (error) {
		std::clog <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}
}

void ConnectStageSession::sendConnectResultHandler(const boost::system::error_code& error, 
												   size_t bytes_transferred) {
	// 如果上一步中连接未成功，就不必创建转发会话了
	if (_connect_result.success) {
		if (!error && bytes_transferred == sizeof(ConnectResult)) {
			boost::shared_ptr<TransferStageSession>(
				new TransferStageSession(_src_socket, _dest_socket))->start();
			boost::shared_ptr<TransferStageSession>(
				new TransferStageSession(_dest_socket, _src_socket))->start();
		} else {
			std::clog <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
		}
	}
}

} // namespace tcp
} // namespace data_transfer
} // namespace gpserver