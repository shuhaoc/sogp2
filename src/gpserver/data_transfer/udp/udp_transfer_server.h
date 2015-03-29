#pragma once
#include "../player_addr_list.h"
#include "transfer_session.h"

namespace gpserver {
namespace data_transfer {
namespace udp {

/**
 * UDP数据转发服务端的RAII类
 */
class UdpTransferServer {
public:
	/**
	 * 构造，并开始服务，需要指定服务占用的UDP端口
	 * @param ios				操作完成的hanlders将通过ios来运行
	 * @param port				服务占用的UDP端口
	 * @param player_addr_list	IPlayerAddrList接口实例
	 */
	UdpTransferServer(boost::asio::io_service& ios, 
					  unsigned short port,
					  IPlayerAddrList& player_addr_list);
	
private:
	/**
	 * 异步等待数据包来临，但不实际接受
	 */
	void asyncPeek();
	
	/**
	 * 数据包来临
	 */
	void peekPacketHandler(boost::shared_ptr<TransferSession> session,
						   const boost::system::error_code& error,
						   size_t bytes_transferred);

	boost::asio::ip::udp::socket _socket;
	IPlayerAddrList& _player_addr_list;
};

} // namespace udp
} // namespace data_transfer
} // namespace gpserver