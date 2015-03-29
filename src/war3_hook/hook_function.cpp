#include "hook_function.h"
#include <iostream>
#include "../utility/ini_config/ini_config.h"
#include "../gpserver/data_transfer/transfer_protocol.h"

#pragma comment(lib, "ws2_32.lib")

namespace war3_hook {

/** TCPת���ķ�������ַ���˿� */
static sockaddr_in __tcp_transfer_server_addr;

/** UDPת���ķ�������ַ���˿� */
static sockaddr_in __udp_transfer_server_addr;

void init(HINSTANCE instance) {
	// �������ļ��ж���������IP��ַ
	utility::IniConfig config(utility::getDirectoryOfModuleFile(instance) + "gpclient.ini", "ServerInfo");
	std::string server_ip = config.readString("IP");
	std::cout << "server is " << server_ip << "sogp2" << std::endl;
	
	// ���뷿��
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
	
	// ��ʼ��TCPת����������ַ
	__tcp_transfer_server_addr.sin_family = AF_INET;
	__tcp_transfer_server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	__tcp_transfer_server_addr.sin_port = htons(1128);
	
	// ��ʼ��UDPת����������ַ
	__udp_transfer_server_addr.sin_family = AF_INET;
	__udp_transfer_server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	__udp_transfer_server_addr.sin_port = htons(1128);
}

/**
 * ������װ�ֵķ�����/����״̬
 */
inline void setSocketNonBlocking(SOCKET s, bool nonblocking) {
	u_long mode = nonblocking;
	ioctlsocket(s, FIONBIO, &mode);
}

SOCKET __stdcall connect(SOCKET s, const sockaddr* name, int namelen) {
	using namespace gpserver::data_transfer;
	std::cout <<  __FUNCTION__ << "-sogp2" << std::endl;
	// ����Ϊ�����׽���
	setSocketNonBlocking(s, false);
	// ����ת��������
	int ret = ::connect(s, (const sockaddr*)&__tcp_transfer_server_addr, sizeof(__tcp_transfer_server_addr));
	
	if (ret != SOCKET_ERROR) {
		// �����������Ŀ���ַ��Ϣ���������ӽ��
		const sockaddr_in* dest_addr = (const sockaddr_in*)name;
		DestinationEndpoint dest_endpoint = {
			dest_addr->sin_addr.s_addr,
			dest_addr->sin_port };
		ConnectResult result = { false };
		
		// ����socket�ո����ӣ���send bufferΪ�գ�����dest_endpoint�ֽ������٣�
		// ����һ����һ����send�����ֽ�
		if (send(s, (const char*)&dest_endpoint, sizeof(dest_endpoint), 0) != sizeof(dest_endpoint)
			|| recv(s, (char*)&result, sizeof(result), MSG_WAITALL) != sizeof(result)
			|| !result.success) {
			// ����send��recvʧ�ܣ����Ƿ��������ص����ӽ��Ϊfalse������Ϊ����ʧ��
			ret = SOCKET_ERROR;
			// �������Ϊ��������������Ŀ�꣬������Ϊ�����ӱ��ܾ�������
			if (!result.success) SetLastError(WSAECONNREFUSED);
		}
	}	
	return ret;
}

int __stdcall sendto(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen) {
	using namespace gpserver::data_transfer;
	std::cout <<  __FUNCTION__ << "-sogp2" << std::endl;
	// ����һ����һ��ͷ����Ϣ��buffer	
	int size = len + sizeof(DestinationEndpoint);
	char* buffer = new char[size];
	// ��дͷ����Ϣ
	sockaddr_in* saddr = (sockaddr_in*)to;
	DestinationEndpoint* header = (DestinationEndpoint*)buffer;
	header->ip32 = saddr->sin_addr.s_addr;
	header->port = saddr->sin_port;
	// ������copy��ȥ
	memcpy(buffer + sizeof(DestinationEndpoint), buf, len);
	// ���͸�������
	int ret = ::sendto(s, buffer, size, flags, (sockaddr*)&__udp_transfer_server_addr, sizeof(sockaddr));
	// �ɹ����ͣ�����ʵ�ʷ����ֽڼ�ͷ�����ȣ�ע����Ҫ���޷�������ת��Ϊ�з�������
	if (ret >= (int)sizeof(DestinationEndpoint)) ret = ret - sizeof(DestinationEndpoint);
	// ����ʧ�ܣ�����SOCKET_ERROR
	else if (ret < 0) ;
	// ͷ����Ϣ���Ͳ�������������������ԣ�����һ���հ�
	else ret = 0;
	delete[] buffer;
	return ret;
}

int __stdcall recvfrom(SOCKET s, char* buf, int len, int flags, sockaddr* from, int* fromlen) {
	using namespace gpserver::data_transfer;
	std::cout <<  __FUNCTION__ << "-sogp2" << std::endl;
	// ����һ����һ��ͷ����Ϣ��buffer	
	int size = len + sizeof(DestinationEndpoint);
	char* buffer = new char[size];
	// ��������
	int ret = ::recvfrom(s, buffer, size, flags, from, fromlen);
	// ��֤�������Է�����ת�������������ϵĵ����հ�����
	sockaddr_in* saddr = (sockaddr_in*)from;
	if (saddr->sin_addr.s_addr != __udp_transfer_server_addr.sin_addr.s_addr 
		|| saddr->sin_port != __udp_transfer_server_addr.sin_port) {
		return 0;
	}
	// ͷ����Ϣ����������ע����Ҫ���޷�������ת��Ϊ�з�������
	if (ret >= (int)sizeof(DestinationEndpoint)) {
		// ����Դ��ַ���˿ڸ�Ϊ��Դ�����ĵ�ַ���˿�
		DestinationEndpoint* header = (DestinationEndpoint*)buffer;
		saddr->sin_addr.s_addr = header->ip32;
		saddr->sin_port = header->port;
		// ʵ�����ݳ���
		int size = ret - sizeof(DestinationEndpoint);
		// ������copy��������
		memcpy(buf, buffer + sizeof(DestinationEndpoint), size);
		ret = size;
	} else if (ret < 0) {
		// ������ܳ�������SOCKET_ERROR
	} else {
		// �������ݲ���ͷ�����ȣ������հ�����
		ret = 0;
	}
	delete[] buffer;
	return ret;
}

} // namespace gphook