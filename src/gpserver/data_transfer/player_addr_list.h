#pragma once
#include <list>

namespace gpserver {
namespace data_transfer {

/**
 * 用于遍历某个范围内所有用户IP址的接口
 */
class IPlayerAddrList {
public:
	/**
	 * 得到范围内所有用户的IP（主机字节序）
	 * @param src_ip	来源主机的IP（主机字节序）
	 */
	virtual std::list<unsigned long> get(unsigned long src_ip) const = 0;
	
	virtual ~IPlayerAddrList() { }

};

} // namespace data_transfer
} // namespace gpserver