#pragma once
#include "../player_addr_list.h"
#include "transfer_session.h"

namespace gpserver {
namespace data_transfer {
namespace udp {

/**
 * UDP����ת������˵�RAII��
 */
class UdpTransferServer {
public:
	/**
	 * ���죬����ʼ������Ҫָ������ռ�õ�UDP�˿�
	 * @param ios				������ɵ�hanlders��ͨ��ios������
	 * @param port				����ռ�õ�UDP�˿�
	 * @param player_addr_list	IPlayerAddrList�ӿ�ʵ��
	 */
	UdpTransferServer(boost::asio::io_service& ios, 
					  unsigned short port,
					  IPlayerAddrList& player_addr_list);
	
private:
	/**
	 * �첽�ȴ����ݰ����٣�����ʵ�ʽ���
	 */
	void asyncPeek();
	
	/**
	 * ���ݰ�����
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