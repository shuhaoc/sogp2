#pragma once
#include "../data_transfer/player_addr_list.h"

namespace gpserver {
namespace players {

/**
 * һ���򵥵ķ��书�ܣ�ά��һ�����������Ӹ����û���socket������Ϊ����
 */
class SimpleRoom : public data_transfer::IPlayerAddrList {
public:
	SimpleRoom(boost::asio::io_service& ios, unsigned short port);
	
	virtual ~SimpleRoom() { }
	
	/**
	 * �첽��������
	 */
	void asyncAccept();
	
	/**
	 * ʵ��IPlayerAddrList�Ľӿڣ��õ���Χ�������û���IP�������ֽ���
	 */	
	virtual std::list<unsigned long> get(unsigned long src_ip) const;
	
private:
	typedef std::list<boost::shared_ptr<boost::asio::ip::tcp::socket> > SocketList;
	
	/**
	 * ������ɣ�����ɹ���������Ӽ���������
	 */
	void acceptHanlder(boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
					   const boost::system::error_code& error);
	
	/**
	 * ������֤�ͻ������ߣ���Ϊ�ͻ��˲��ᷢ���κ����ݣ����첽�����������ʱ����ζ�����ӹرգ�
	 * ���Ὣ�����Ӵ�������ɾ��
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