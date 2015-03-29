#include "stdafx.h"
#include "transfer_session.h"
#include "../transfer_protocol.h"

namespace gpserver {
namespace data_transfer {
namespace udp {

TransferSession::TransferSession(gpserver::data_transfer::IPlayerAddrList &player_addr_list, 
								 boost::asio::ip::udp::socket &socket)
	: _player_addr_list(player_addr_list), _socket(socket), _size(0) {
}

void TransferSession::start() {
	try {
		//std::cout << __FUNCTION__ << std::endl;		
		// ��ȡ���ݰ���С�����˷��仺�����ڴ�
		size_t capacity = _socket.available();
		_buf.reset(new char[capacity]);
		
		// �ӻ�����ж��׻�ȡ�����ݰ�����Ȼpeek�ܹ��ɹ���ֱ��receive_fromӦ�ò�������
		_size = _socket.receive_from(boost::asio::buffer(_buf.get(), capacity), _src_endpoint);
		
		DestinationEndpoint dest_endpoint;
		// ���ݰ�����Ӧ��С��ͷ������
		if (_size >= sizeof(dest_endpoint)) {
			DestinationEndpoint* header = (DestinationEndpoint*)_buf.get();
			// ��ȡ��Ŀ��������Ϣ
			dest_endpoint = *header;
			// ��ͷ���޸�Ϊ��Դ������ַ���˿�
			unsigned long src_ip = _src_endpoint.address().to_v4().to_ulong();
			header->ip32 = htonl(src_ip);
			header->port = htons(_src_endpoint.port());
			
			// ����ǹ㲥��ַ��ת����һ����Χ�������û�
			if (ntohl(dest_endpoint.ip32) == boost::asio::ip::address_v4::broadcast().to_ulong()) {
				typedef std::list<unsigned long> AddressList;
				AddressList addr_list = _player_addr_list.get(src_ip);
				for (AddressList::iterator itr = addr_list.begin(); itr != addr_list.end(); itr++) {
					sendDataTo(*itr, ntohs(dest_endpoint.port));
					std::cout << "send to " << std::hex << *itr << std::dec << std::endl;
				}
			} else {
				// ��ͨ��ַ��ֱ�ӷ��͸�Ŀ������
				sendDataTo(ntohl(dest_endpoint.ip32), ntohs(dest_endpoint.port));
			}					
		}
	} catch (boost::system::system_error& error) {
		std::clog << __FUNCTION__ << error.code() << error.what() << "-sogp2" << std::endl;
	}
}

void TransferSession::sendDataTo(unsigned long ip, unsigned short port) {
	_socket.async_send_to(boost::asio::buffer(_buf.get(), _size),
						  boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4(ip), port),
						  boost::bind(&TransferSession::sendDataToDestHandler,
									  shared_from_this(),
									  boost::asio::placeholders::error,
									  boost::asio::placeholders::bytes_transferred));
}

void TransferSession::sendDataToDestHandler(const boost::system::error_code &error, 
											size_t bytes_transfferred) {
	if (!error && bytes_transfferred) {											
		// ʲô������Ҫ����������������
		//std::cout << __FUNCTION__ << std::endl;
	} else {
		std::clog << __FUNCTION__ << error.value() << error.message() << "-sogp2" << std::endl;		
	}
}

} // namespace udp
} // namespace data_transfer
} // namespace gpserver