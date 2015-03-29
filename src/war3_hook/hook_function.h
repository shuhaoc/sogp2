#pragma once
#include <Winsock2.h>

namespace war3_hook {

/**
 * ��ʼ��������Ϣ
 */
void init(HINSTANCE instance);

// -----------------------------------------------------------------------------------------
// ���war3��Hook����
/**
 * Winsock��connect������HOOK�汾��Ӧ����TCP����ת�����˺����ڻὫs����Ϊ����ģʽ
 */
SOCKET __stdcall connect(SOCKET s, const sockaddr* name, int namelen);

/**
 * Winsock��sendto������HOOK�汾��Ӧ����UDP����ת��
 */
int __stdcall sendto(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen);

/**
 * Winsock��recvfrom������HOOK�汾��Ӧ����UDP����ת��
 */
int __stdcall recvfrom(SOCKET s, char* buf, int len, int flags, sockaddr* from, int* fromlen);

} // namespace war3_hook