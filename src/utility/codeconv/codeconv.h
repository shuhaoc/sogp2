#pragma once
#include <string>

namespace utility {
namespace codeconv {

/**
 * ANSI���루GB2312���ַ���ת��ΪUnicode���루UTF-16����
 * @param src	���������Դ�ַ���
 * @param dest	���������Ŀ���ַ���
 */
void ansiToUnicode(const std::string& src, std::wstring& dest);

/**
 * Unicode���루UTF-16���ַ���ת��ΪANSI���루GB2312����
 * @param src	���������Դ�ַ���
 * @param dest	���������Ŀ���ַ���
 */
void unicodeToAnsi(const std::wstring& src, std::string& dest);
	
} // namespace codeconv
} // namespace utility