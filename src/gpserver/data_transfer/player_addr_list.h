#pragma once
#include <list>

namespace gpserver {
namespace data_transfer {

/**
 * ���ڱ���ĳ����Χ�������û�IPַ�Ľӿ�
 */
class IPlayerAddrList {
public:
	/**
	 * �õ���Χ�������û���IP�������ֽ���
	 * @param src_ip	��Դ������IP�������ֽ���
	 */
	virtual std::list<unsigned long> get(unsigned long src_ip) const = 0;
	
	virtual ~IPlayerAddrList() { }

};

} // namespace data_transfer
} // namespace gpserver