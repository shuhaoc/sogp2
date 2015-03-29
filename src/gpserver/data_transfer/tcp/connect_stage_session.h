#pragma once
#include "../transfer_protocol.h"

namespace gpserver {
namespace data_transfer {
namespace tcp {

/**
 * TCP转发会话的连接阶段，也是第一阶段。对象必须通过new创建并交由shared_ptr管理。
 * 转发服务端（S）aync_accept前会new一个此类对象，当S成功接受来自转发发起者（用
 * 户A）的连接，应该调用start方法开始此会话处理程序。会话将分为三步：A向S发送目
 * 标用户（用户B）的DestinationEndpoint、S连接B、S向A发送连接连接结果ConnectResult，
 * 参见../transfer_protocol.h中的定义。
 * 当最后一步完成，会创建和开始一对转发阶段会话TransferStageSession。
 */
class ConnectStageSession : public boost::enable_shared_from_this<ConnectStageSession>, 
							private boost::noncopyable {
public:
	/**
	 * 构造，双向的tcp::socket都将在这里创建
	 */
	ConnectStageSession(boost::asio::io_service& ios);
	
	/**
	 * 获取与发起者用户连接的tcp::socket
	 */
	boost::asio::ip::tcp::socket& src_socket() { return *_src_socket; }

	/**
	 * aync_accept成功后，才能开始会话中的异步处理。
	 */
	void start();

private:
	/**
	 * 接受A发送的B的DestinationEndpoint完成，开始连接B
	 */
	void recvDestEndpointHandler(const boost::system::error_code& error,
								 size_t bytes_transferred);

	/**
	 * 连接B完成，开始向A发送连接结果
	 */
	void connectDestinationHandler(const boost::system::error_code& error);

	/**
	 * 向A发送连接结果完成，创建和启动一对转发阶段会话
	 */
	void sendConnectResultHandler(const boost::system::error_code& error,
								  size_t bytes_transferred);

	boost::shared_ptr<boost::asio::ip::tcp::socket> _src_socket;
	boost::shared_ptr<boost::asio::ip::tcp::socket> _dest_socket;
	DestinationEndpoint _dest_endpoint;
	ConnectResult _connect_result;
};

} // namespace tcp
} // namespace data_transfer
} // namespace gpserver