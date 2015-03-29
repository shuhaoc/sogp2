#pragma once

namespace gpserver {
namespace data_transfer {

/**
 * 目标地址、端口信息，在TCP、UDP转发中都有使用。
 * 一、在TCP转发中，当用户A需要连接用户B时，A会被HOOK以至连接转发服务器S，
 *	   然后A需要以此发送B的地址、端口信息。
 * 二、在UDP转发中，当A需要发送给B一个数据包时，A将会被HOOK以至在数据包前
 *     端插入一个对象的数据；当数据包到了S时，S将会把数据包的头部替换成A的
 *     DestinationEndpoint然后转发给B；B收到数据包时会被HOOK以至从数据包头
 *     部提取出来源地址及端口。
 */
struct DestinationEndpoint {

	/** 网络字节序32位IPV4地址 */
	unsigned long ip32;
	
	/** 网络字节序的端口号 */
	unsigned short port;
};

/**
 * 当用户A连接转发服务器S并发送目标B的地址端口信息（DestinationEndpoint），
 * S连接用户B，ConnectResult表示S连接B的成功与否，S将会将此对象返回给A。
 */
struct ConnectResult {

	// 连接是否成功c
	bool success;
};

} // namespace data_transfer
} // namespace gpserver