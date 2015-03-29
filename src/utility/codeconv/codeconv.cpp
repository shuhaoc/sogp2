#include "codeconv.h"
#include <Windows.h>

/**
 * ���ֽڱ����ַ���ת��ΪUnicode���롣
 * @param src_code_page		Դ�������ͣ��μ�MSDN��MultiByteToWideChar�е�һ������
 * @param src				���������Դ���ֽ��ַ���
 * @param dest				���������Ŀ��Unicode�ַ���
 * @see						�μ�MSDN��MultiByteToWideChar����
 */
void multiByteToWideChar(UINT src_code_page, const std::string& src, std::wstring& dest) {
	int wide_chars_num = MultiByteToWideChar(src_code_page, 
											 0, 
											 src.c_str(), 
											 src.size(), 
											 NULL, 
											 0);
	dest.resize(wide_chars_num);
	
	MultiByteToWideChar(src_code_page, 
						0, 
						src.c_str(), 
						src.size(), 
						const_cast<wchar_t*>(dest.c_str()),
						dest.size());
}

/**
 * Unicode�����ַ���ת��Ϊ���ֽڱ��롣
 * @param src_code_page		Ŀ��������ͣ��μ�MSDN��MultiByteToWideChar�е�һ������
 * @param src				���������ԴUnicode�ַ���
 * @param dest				���������Ŀ����ֽ��ַ�����ָ��
 * @see						�μ�MSDN��MultiByteToWideChar����
 */
void wideCharToMultiByte(UINT dest_code_page, const std::wstring& src, std::string& dest) {
	int multi_bytes_num = WideCharToMultiByte(dest_code_page,
											  0,
											  src.c_str(),
											  src.size(),
											  NULL,
											  NULL,
											  NULL,
											  NULL);
	dest.resize(multi_bytes_num);
	
	WideCharToMultiByte(dest_code_page,
						0,
						src.c_str(),
						src.size(),
						const_cast<char*>(dest.c_str()),
						dest.size(),
						NULL,
						NULL);
}

// -------------------------------------------------------------------------------------------

namespace utility {
namespace codeconv {

void ansiToUnicode(const std::string& src, std::wstring& dest) {
	multiByteToWideChar(CP_ACP, src, dest);
}

void unicodeToAnsi(const std::wstring& src, std::string& dest) {
	wideCharToMultiByte(CP_ACP, src, dest);
}
		
} // namespace codeconv
} // namespace utility