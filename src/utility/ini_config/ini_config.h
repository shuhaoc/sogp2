#pragma once
#include <string>

namespace utility {

/**
 * 读取ini配置文件中配置项的类
 */
class IniConfig {
public:
	/**
	 * 构造
	 * @param file		配置文件名，绝对路径
	 * @param section	配置节名
	 */
	IniConfig(const std::string& file, const std::string& section);
	
	/**
	 * 读取字符串值，失败返回空字符串
	 * @param key		配置键名
	 * @param max_len	字符串最大长度，如果实际值超过此值将会被截断
	 */
	std::string readString(const std::string& key, int max_len = 256) const;
	
	/**
	 * 读取整形值，失败返回0
	 * @param key		配置键名
	 */
	int readInt(const std::string& key) const;
	
private:
	std::string _file_name;
	std::string _section_name;
};

/**
 * 获取模块文件所在文件夹名，包含最后的'\\'，失败返回空字符串
 * @param module_handle	模块句柄，NULL表示本进程的exe文件
 */
std::string getDirectoryOfModuleFile(void* module_handle = NULL);

} // namespace utility