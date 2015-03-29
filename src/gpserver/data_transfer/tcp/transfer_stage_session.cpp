#include "stdafx.h"
#include "transfer_stage_session.h"

namespace gpserver {
namespace data_transfer {
namespace tcp {

/** 转发缓冲区大小 */
static const int BUF_SIZE = 1024;

TransferStageSession::TransferStageSession(boost::shared_ptr<boost::asio::ip::tcp::socket> src_socket,
										   boost::shared_ptr<boost::asio::ip::tcp::socket> dest_socket) 
	: _buf(new char[BUF_SIZE]), _src_socket(src_socket), _dest_socket(dest_socket) {
}

TransferStageSession::~TransferStageSession() {
	// 关闭两个双向的连接
	_src_socket->close();
	_dest_socket->close();
}

void TransferStageSession::start() {
	//std::cout << __FUNCTION__ << std::endl;
	// 开始从发送方异步接受数据
	_src_socket->async_receive(
		boost::asio::buffer(_buf.get(), BUF_SIZE),
		boost::bind(&TransferStageSession::recvSrcDataHandler,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void TransferStageSession::recvSrcDataHandler(const boost::system::error_code &error, 
											  size_t bytes_transferred) {
	if (!error && bytes_transferred) {
		// 开始向目标方发送该数据
		_dest_socket->async_send(
			boost::asio::buffer(_buf.get(), bytes_transferred),
			boost::bind(&TransferStageSession::sendDestDataHandler,
						shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	} else {
		std::cout <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}
}

void TransferStageSession::sendDestDataHandler(const boost::system::error_code &error, 
											   size_t bytes_transferred) {
	if (!error && bytes_transferred) {
		// 继续从来源方接受数据
		start();
	} else {
		std::cout <<  __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;
	}
}

} // namespace tcp
} // namespace data_transfer
} // namespace gpserver