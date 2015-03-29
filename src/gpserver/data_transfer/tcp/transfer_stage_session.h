#pragma once

namespace gpserver {
namespace data_transfer {
namespace tcp {

/**
 * TCP转发会话的数据转发阶段，也是第二阶段，实现两个用户之间单向的转发。对象必须
 * 通过new创建并将由shared_ptr管理，而且是同时创建一对。
 * 当服务端（S）与两个用户（来源方A、目标方B）都已经建立连接之后，从与A连接的tcp::socket
 * 接受到数据，然后原封不动地发送给B。同时与自己对应的另一个对象实现反向的转发。
 * 当任何一端出错，会关闭两个tcp::socket，同时与自己对应的另一个对象
 * 会因连接被关闭而出错，以至结束转发并析构。
 */
class TransferStageSession : public boost::enable_shared_from_this<TransferStageSession>,
							 private boost::noncopyable {
public:
	/**
	 * 构造，仅初始化数据成员
	 */
	TransferStageSession(boost::shared_ptr<boost::asio::ip::tcp::socket> src_socket,
						 boost::shared_ptr<boost::asio::ip::tcp::socket> dest_socket);
						 
	/**
	 * 析构，关闭两个tcp::socket
	 */
	~TransferStageSession();

	/**
	 * 开始从发送方异步接受数据
	 */
	void start();
		
private:
	/**
	 * 从来源方接受数据完成，开始发送给目标方
	 */
	void recvSrcDataHandler(const boost::system::error_code& error,
							size_t bytes_transferred);
						 
	/**
	 * 向目标方发送完成，继续从来源方接受数据
	 */
	void sendDestDataHandler(const boost::system::error_code& error,
							size_t bytes_transferred);						 
						 			
	boost::scoped_array<char> _buf;
	boost::shared_ptr<boost::asio::ip::tcp::socket> _src_socket;
	boost::shared_ptr<boost::asio::ip::tcp::socket> _dest_socket;
};		
		
} // namespace tcp
} // namespace data_transfer
} // namespace gpserver