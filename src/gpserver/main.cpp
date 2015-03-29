#include "stdafx.h"
#include "data_transfer/tcp/tcp_transfer_server.h"
#include "data_transfer/udp/udp_transfer_server.h"
#include "players/simple_room.h"
//#include "../utility/debugging/debugstream.h"

//class TestRoom : public gpserver::data_transfer::IPlayerAddrList {
//public:
//	std::list<unsigned long> get() const {
//		std::list<unsigned long> ret;
//		ret.push_back(ntohl(inet_addr("192.168.255.1")));
//		ret.push_back(ntohl(inet_addr("192.168.255.129")));
//		return ret;
//	}
//	
//	~TestRoom() { }
//};

void main() {
	//std::cout.rdbuf(utility::debug.rdbuf());
	
	try {
		// ����ֻ����һ��io_service������ζ�����еĲ���������ͬһ�߳�����У�
		// ��������ڶ��ϵͳ�����У����ö���̵߳���io_service��run���������������ܣ�
		// �����ĳ��ģ�鲻�����̰߳�ȫ��֤������������io_service��Ӧ�����̵߳�����run������
		boost::asio::io_service ios;
		gpserver::data_transfer::tcp::TcpTransferServer tcp_transfer_server(ios, 1128);
		gpserver::players::SimpleRoom room(ios, 1129);
		gpserver::data_transfer::udp::UdpTransferServer udp_transfer_server(ios, 1128, room);
		ios.run();
	} catch (std::exception& error) {
		std::cout << __FUNCTION__ << error.what() << std::endl;
		system("pause");
	}
}

