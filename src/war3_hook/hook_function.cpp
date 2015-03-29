#include "hook_function.h"
#include <iostream>
#include "../utility/ini_config/ini_config.h"
#include "../gpserver/data_transfer/transfer_protocol.h"

#pragma comment(lib, "ws2_32.lib")

namespace war3_hook {

/** TCP转发的服务器地址、端口 */
static sockaddr_in __tcp_transfer_server_addr;

/** UDP转发的服务器地址、端口 */
static sockaddr_in __udp_transfer_server_addr;

void init(HINSTANCE instance) {
	// 从配置文件中读出服务器IP地址
	utility::IniConfig config(utility::getDirectoryOfModuleFile(instance) + "gpclient.ini", "ServerInfo");
	std::string server_ip = config.readString("IP");
	std::cout << "server is " << server_ip << "sogp2" << std::endl;
	
	// 加入房间
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in room_server_addr;
	room_server_addr.sin_family = AF_INET;
	room_server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	room_server_addr.sin_port = htons(1129);
	if (::connect(client_socket, (sockaddr*)&room_server_addr, sizeof(sockaddr)) == SOCKET_ERROR) {
		std::cout << "connect server error " << WSAGetLastError() << "-sogp2" << std::endl;
	}
	
	// 初始化TCP转化服务器地址
	__tcp_transfer_server_addr.sin_family = AF_INET;
	__tcp_transfer_server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	__tcp_transfer_server_addr.sin_port = htons(1128);
	
	// 初始化UDP转化服务器地址
	__udp_transfer_server_addr.sin_family = AF_INET;
	__udp_transfer_server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	__udp_transfer_server_addr.sin_port = htons(1128);
}

/**
 * 设置套装字的非阻塞/阻塞状态
 */
inline void setSocketNonBlocking(SOCKET s, bool nonblocking) {
	u_long mode = nonblocking;
	ioctlsocket(s, FIONBIO, &mode);
}

SOCKET __stdcall connect(SOCKET s, const sockaddr* name, int namelen) {
	using namespace gpserver::data_transfer;
	std::cout <<  __FUNCTION__ << "-sogp2" << std::endl;
	// 设置为阻塞套接字
	setSocketNonBlocking(s, false);
	// 连接转发服务器
	int ret = ::connect(s, (const sockaddr*)&__tcp_transfer_server_addr, sizeof(__tcp_transfer_server_addr));
	
	if (ret != SOCKET_ERROR) {
		// 向服务器发送目标地址信息、接受连接结果
		const sockaddr_in* dest_addr = (const sockaddr_in*)name;
		DestinationEndpoint dest_endpoint = {
			dest_addr->sin_addr.s_addr,
			dest_addr->sin_port };
		ConnectResult result = { false };
		
		// 由于socket刚刚连接，其send buffer为空，而且dest_endpoint字节数较少，
		// 所以一定能一次性send所有字节
		if (send(s, (const char*)&dest_endpoint, sizeof(dest_endpoint), 0) != sizeof(dest_endpoint)
			|| recv(s, (char*)&result, sizeof(result), MSG_WAITALL) != sizeof(result)
			|| !result.success) {
			// 无论send或recv失败，还是服务器返回的连接结果为false，都视为连接失败
			ret = SOCKET_ERROR;
			// 如果是因为服务器不能连接目标，将其设为“连接被拒绝”错误
			if (!result.success) SetLastError(WSAECONNREFUSED);
		}
	}	
	return ret;
}

int __stdcall sendto(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen) {
	using namespace gpserver::data_transfer;
	std::cout <<  __FUNCTION__ << "-sogp2" << std::endl;
	// 创建一个多一个头部信息的buffer	
	int size = len + sizeof(DestinationEndpoint);
	char* buffer = new char[size];
	// 填写头部信息
	sockaddr_in* saddr = (sockaddr_in*)to;
	DestinationEndpoint* header = (DestinationEndpoint*)buffer;
	header->ip32 = saddr->sin_addr.s_addr;
	header->port = saddr->sin_port;
	// 把数据copy过去
	memcpy(buffer + sizeof(DestinationEndpoint), buf, len);
	// 发送给服务器
	int ret = ::sendto(s, buffer, size, flags, (sockaddr*)&__udp_transfer_server_addr, sizeof(sockaddr));
	// 成功发送，返回实际发送字节减头部长度，注意需要将无符号整型转化为有符号整型
	if (ret >= (int)sizeof(DestinationEndpoint)) ret = ret - sizeof(DestinationEndpoint);
	// 调用失败，返回SOCKET_ERROR
	else if (ret < 0) ;
	// 头部信息发送不完整，服务器将会忽略，当作一个空包
	else ret = 0;
	delete[] buffer;
	return ret;
}

int __stdcall recvfrom(SOCKET s, char* buf, int len, int flags, sockaddr* from, int* fromlen) {
	using namespace gpserver::data_transfer;
	std::cout <<  __FUNCTION__ << "-sogp2" << std::endl;
	// 创建一个多一个头部信息的buffer	
	int size = len + sizeof(DestinationEndpoint);
	char* buffer = new char[size];
	// 接受数据
	int ret = ::recvfrom(s, buffer, size, flags, from, fromlen);
	// 验证数据来自服务器转发而来，不符合的当作空包处理
	sockaddr_in* saddr = (sockaddr_in*)from;
	if (saddr->sin_addr.s_addr != __udp_transfer_server_addr.sin_addr.s_addr 
		|| saddr->sin_port != __udp_transfer_server_addr.sin_port) {
		return 0;
	}
	// 头部信息必须完整，注意需要将无符号整型转化为有符号整型
	if (ret >= (int)sizeof(DestinationEndpoint)) {
		// 把来源地址、端口改为来源主机的地址、端口
		DestinationEndpoint* header = (DestinationEndpoint*)buffer;
		saddr->sin_addr.s_addr = header->ip32;
		saddr->sin_port = header->port;
		// 实际数据长度
		int size = ret - sizeof(DestinationEndpoint);
		// 把数据copy给调用者
		memcpy(buf, buffer + sizeof(DestinationEndpoint), size);
		ret = size;
	} else if (ret < 0) {
		// 如果接受出错，返回SOCKET_ERROR
	} else {
		// 接受数据不足头部长度，当作空包处理
		ret = 0;
	}
	delete[] buffer;
	return ret;
}

} // namespace gphook