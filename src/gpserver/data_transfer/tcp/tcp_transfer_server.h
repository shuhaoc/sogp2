#pragma once
#include "connect_stage_session.h"

namespace gpserver {
namespace data_transfer {
namespace tcp {

/**
 * TCP数据转发服务端的RAII类
 */
class TcpTransferServer : private boost::noncopyable {
public:
	/**
	 * 构造并开始服务，需要指定服务端占用的TCP端口
	 * @param ios		操作完成的hanlders将通过ios来运行
	 * @param port		服务占用的TCP端口
	 */
	TcpTransferServer(boost::asio::io_service& ios, unsigned short port);
		
private:
	/**
	 * 异步接受连接
	 */
	void asyncAccept();
	
	/**
	 * 接受连接完成，连接阶段会话开始。
	 */
	void acceptHandler(boost::shared_ptr<ConnectStageSession> session,
					   const boost::system::error_code& error);
					   
	boost::asio::io_service& _ios;
	boost::asio::ip::tcp::acceptor _acceptor;					   
};

} // namespace tcp
} // namespace data_transfer
} // namespace gpserver