#include "ini_config.h"
#include <Windows.h>

namespace utility {

std::string getDirectoryOfModuleFile(void* module_handle) {
	// 先获取文件绝对路径，再去掉文件名得到文件夹路径
	std::string module_file(MAX_PATH, '\0');
	size_t size = GetModuleFileName((HMODULE)module_handle, const_cast<char*>(module_file.c_str()), module_file.capacity());
	module_file.resize(size);
	size_t index = module_file.find_last_of('\\');
	return module_file.substr(0, index + 1);
}

IniConfig::IniConfig(const std::string &file, const std::string &section) 
	: _file_name(file), _section_name(section) {
}

std::string IniConfig::readString(const std::string &key, int max_len/* = 256*/) const {
	std::string value(max_len, '\0');
	size_t size = GetPrivateProfileString(_section_name.c_str(), 
										  key.c_str(), 
										  "",	
										  const_cast<char*>(value.c_str()),
										  value.capacity(),
										  _file_name.c_str());
	value.resize(size);										  										  
	return value;						
}

int IniConfig::readInt(const std::string &key) const {
	return GetPrivateProfileInt(_section_name.c_str(),
								key.c_str(),
								0,
								_file_name.c_str());
}


} // namespace utility