#pragma once
#include <iostream>

namespace utility {

/**
 * �����������������ݵ�ostream
 */
class DebugStream : public std::ostream {
public:
	DebugStream();
	
	virtual ~DebugStream();
};

/** DebugStream��ȫ��ʵ�� */
extern DebugStream debug;

} // namespace utility