#pragma once
#include <string>

namespace utility {

/**
 * ��ȡini�����ļ������������
 */
class IniConfig {
public:
	/**
	 * ����
	 * @param file		�����ļ���������·��
	 * @param section	���ý���
	 */
	IniConfig(const std::string& file, const std::string& section);
	
	/**
	 * ��ȡ�ַ���ֵ��ʧ�ܷ��ؿ��ַ���
	 * @param key		���ü���
	 * @param max_len	�ַ�����󳤶ȣ����ʵ��ֵ������ֵ���ᱻ�ض�
	 */
	std::string readString(const std::string& key, int max_len = 256) const;
	
	/**
	 * ��ȡ����ֵ��ʧ�ܷ���0
	 * @param key		���ü���
	 */
	int readInt(const std::string& key) const;
	
private:
	std::string _file_name;
	std::string _section_name;
};

/**
 * ��ȡģ���ļ������ļ���������������'\\'��ʧ�ܷ��ؿ��ַ���
 * @param module_handle	ģ������NULL��ʾ�����̵�exe�ļ�
 */
std::string getDirectoryOfModuleFile(void* module_handle = NULL);

} // namespace utility