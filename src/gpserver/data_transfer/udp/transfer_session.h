#pragma once
#include "../transfer_protocol.h"
#include "../player_addr_list.h"

namespace gpserver {
namespace data_transfer {
namespace udp {

/**
 * UDP转发会话，当TransferServer成功等待一个数据包来临后，TransferSession
 * 负责接受此数据包（来自主机A）并发送给目标主机（B）。
 * 当收到A发送的数据包后，提取数据包头部的DestinationEndpoint作为B的地址、端口
 * 信息，然后把据包的头部替换成A的DestinationEndpoint并转发给B。如果目标地址是
 * 广播地址（255.255.255.255），会将此数据包转发给IPlayerAddrList::get返回的范
 * 围内的所有用户。
 */
struct TransferSession : public boost::enable_shared_from_this<TransferSession>,
						 private boost::noncopyable {	
public:
	/**
	 * 构造，仅初始化数据成员
	 */
	TransferSession(IPlayerAddrList& player_addr_list,
					boost::asio::ip::udp::socket& socket);
	
	/**
	 * 开始处理数据并异步发送给目标
	 */
	void start();
	
	/**
	 * 来源主机的地址、端口
	 */
	boost::asio::ip::udp::endpoint& src_endpoint() { return _src_endpoint; }
	
private:
	/**
	 * 发送数据包至目标主机完成，会话也在此结束
	 */
	void sendDataToDestHandler(const boost::system::error_code& error,
							   size_t bytes_transfferred);								

	/**
	 * 发送处理好的数据到目标主机
	 * @param ip	主机字节序的IP地址
	 * @param port	主机字节序的端口号
	 */
	void sendDataTo(unsigned long ip, unsigned short port);

	boost::scoped_ptr<char> _buf;
	size_t _size;
	boost::asio::ip::udp::endpoint _src_endpoint;
	boost::asio::ip::udp::socket& _socket;
	IPlayerAddrList& _player_addr_list;
};

} // namespace udp
} // namespace data_transfer
} // namespace gpserver