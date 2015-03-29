#pragma once
#include <iostream>

namespace utility {

/**
 * 用于向调试器输出内容的ostream
 */
class DebugStream : public std::ostream {
public:
	DebugStream();
	
	virtual ~DebugStream();
};

/** DebugStream的全局实例 */
extern DebugStream debug;

} // namespace utility