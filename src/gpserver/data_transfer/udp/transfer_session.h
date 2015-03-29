#pragma once
#include "../transfer_protocol.h"
#include "../player_addr_list.h"

namespace gpserver {
namespace data_transfer {
namespace udp {

/**
 * UDPת���Ự����TransferServer�ɹ��ȴ�һ�����ݰ����ٺ�TransferSession
 * ������ܴ����ݰ�����������A�������͸�Ŀ��������B����
 * ���յ�A���͵����ݰ�����ȡ���ݰ�ͷ����DestinationEndpoint��ΪB�ĵ�ַ���˿�
 * ��Ϣ��Ȼ��Ѿݰ���ͷ���滻��A��DestinationEndpoint��ת����B�����Ŀ���ַ��
 * �㲥��ַ��255.255.255.255�����Ὣ�����ݰ�ת����IPlayerAddrList::get���صķ�
 * Χ�ڵ������û���
 */
struct TransferSession : public boost::enable_shared_from_this<TransferSession>,
						 private boost::noncopyable {	
public:
	/**
	 * ���죬����ʼ�����ݳ�Ա
	 */
	TransferSession(IPlayerAddrList& player_addr_list,
					boost::asio::ip::udp::socket& socket);
	
	/**
	 * ��ʼ�������ݲ��첽���͸�Ŀ��
	 */
	void start();
	
	/**
	 * ��Դ�����ĵ�ַ���˿�
	 */
	boost::asio::ip::udp::endpoint& src_endpoint() { return _src_endpoint; }
	
private:
	/**
	 * �������ݰ���Ŀ��������ɣ��ỰҲ�ڴ˽���
	 */
	void sendDataToDestHandler(const boost::system::error_code& error,
							   size_t bytes_transfferred);								

	/**
	 * ���ʹ���õ����ݵ�Ŀ������
	 * @param ip	�����ֽ����IP��ַ
	 * @param port	�����ֽ���Ķ˿ں�
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