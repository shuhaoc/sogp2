#pragma once
#include "../data_transfer/player_addr_list.h"

namespace gpserver {
namespace players {

/**
 * 一个简单的房间功能，维持一个链表保存连接各个用户的socket，仅作为测试
 */
class SimpleRoom : public data_transfer::IPlayerAddrList {
public:
	SimpleRoom(boost::asio::io_service& ios, unsigned short port);
	
	virtual ~SimpleRoom() { }
	
	/**
	 * 异步接受连接
	 */
	void asyncAccept();
	
	/**
	 * 实现IPlayerAddrList的接口，得到范围内所有用户的IP（主机字节序）
	 */	
	virtual std::list<unsigned long> get(unsigned long src_ip) const;
	
private:
	typedef std::list<boost::shared_ptr<boost::asio::ip::tcp::socket> > SocketList;
	
	/**
	 * 连接完成，如果成功将会把连接加入链表中
	 */
	void acceptHanlder(boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
					   const boost::system::error_code& error);
	
	/**
	 * 用于验证客户端在线，因为客户端不会发送任何数据，把异步接受数据完成时即意味着连接关闭，
	 * 将会将此连接从链表中删除
	 */
	void receiveHandler(SocketList::iterator socket_iterator,
						const boost::system::error_code& error,
						size_t bytes_transferred);

	boost::asio::io_service& _ios;
	boost::asio::ip::tcp::acceptor _acceptor;
	SocketList _client_list;
};

} // namespace players
} // namespace gpserver