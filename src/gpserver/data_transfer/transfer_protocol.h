#pragma once

namespace gpserver {
namespace data_transfer {

/**
 * Ŀ���ַ���˿���Ϣ����TCP��UDPת���ж���ʹ�á�
 * һ����TCPת���У����û�A��Ҫ�����û�Bʱ��A�ᱻHOOK��������ת��������S��
 *	   Ȼ��A��Ҫ�Դ˷���B�ĵ�ַ���˿���Ϣ��
 * ������UDPת���У���A��Ҫ���͸�Bһ�����ݰ�ʱ��A���ᱻHOOK���������ݰ�ǰ
 *     �˲���һ����������ݣ������ݰ�����Sʱ��S��������ݰ���ͷ���滻��A��
 *     DestinationEndpointȻ��ת����B��B�յ����ݰ�ʱ�ᱻHOOK���������ݰ�ͷ
 *     ����ȡ����Դ��ַ���˿ڡ�
 */
struct DestinationEndpoint {

	/** �����ֽ���32λIPV4��ַ */
	unsigned long ip32;
	
	/** �����ֽ���Ķ˿ں� */
	unsigned short port;
};

/**
 * ���û�A����ת��������S������Ŀ��B�ĵ�ַ�˿���Ϣ��DestinationEndpoint����
 * S�����û�B��ConnectResult��ʾS����B�ĳɹ����S���Ὣ�˶��󷵻ظ�A��
 */
struct ConnectResult {

	// �����Ƿ�ɹ�c
	bool success;
};

} // namespace data_transfer
} // namespace gpserver