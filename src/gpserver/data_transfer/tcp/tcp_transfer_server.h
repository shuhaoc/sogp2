#pragma once
#include "connect_stage_session.h"

namespace gpserver {
namespace data_transfer {
namespace tcp {

/**
 * TCP����ת������˵�RAII��
 */
class TcpTransferServer : private boost::noncopyable {
public:
	/**
	 * ���첢��ʼ������Ҫָ�������ռ�õ�TCP�˿�
	 * @param ios		������ɵ�hanlders��ͨ��ios������
	 * @param port		����ռ�õ�TCP�˿�
	 */
	TcpTransferServer(boost::asio::io_service& ios, unsigned short port);
		
private:
	/**
	 * �첽��������
	 */
	void asyncAccept();
	
	/**
	 * ����������ɣ����ӽ׶λỰ��ʼ��
	 */
	void acceptHandler(boost::shared_ptr<ConnectStageSession> session,
					   const boost::system::error_code& error);
					   
	boost::asio::io_service& _ios;
	boost::asio::ip::tcp::acceptor _acceptor;					   
};

} // namespace tcp
} // namespace data_transfer
} // namespace gpserver