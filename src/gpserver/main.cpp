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
		// 这里只用了一个io_service，这意味着所有的操作将会在同一线程里进行；
		// 如果程序在多核系统上运行，采用多个线程调用io_service的run方法将会提升性能；
		// 但如果某个模块不具有线程安全保证，则其依赖的io_service不应被多线程调用其run方法。
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

