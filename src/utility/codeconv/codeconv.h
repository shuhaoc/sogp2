#pragma once
#include <string>

namespace utility {
namespace codeconv {

/**
 * ANSI编码（GB2312）字符串转化为Unicode编码（UTF-16）。
 * @param src	输入参数，源字符串
 * @param dest	输出参数，目标字符串
 */
void ansiToUnicode(const std::string& src, std::wstring& dest);

/**
 * Unicode编码（UTF-16）字符串转化为ANSI编码（GB2312）。
 * @param src	输入参数，源字符串
 * @param dest	输出参数，目标字符串
 */
void unicodeToAnsi(const std::wstring& src, std::string& dest);
	
} // namespace codeconv
} // namespace utility