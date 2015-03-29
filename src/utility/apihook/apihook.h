#pragma once
#include <string>
#include <exception>

namespace utility {
namespace apihook {

/**
 * ʹ��EasyHook����ܷ������쳣
 */
class EasyhookException : public std::exception {
public:
	EasyhookException(const std::string& message) : _message(message) { }
	
	virtual const char* what() const throw() { return _message.c_str(); }
	
private:
	std::string _message;
};


/**
 * Hookָ���ĺ���������Ӱ�쵱ǰ�̡߳�
 * @param module			DLLģ����
 * @param function_name		ҪHook�ĺ�����
 * @param hook_function		Hook����ָ��
 * @exception				EasyhookException
 */
void InstallHookForThisThread(void* module, const char* function_name, void* hook_function);

/**
 * �������̣�����32λ��������ע��ָ��DLL�ļ��������½��̵�PID��
 * ע���DLL��Ҫ��NativeInjectionEntryPoint�ӿڣ��˽ӿ���easyhook_utility.cpp��
 * �Ѿ�ʵ�֣���NativeInjectionEntryPoint�����ỽ�Ѵ˽��̡�
 * @param exe_path		��ִ���ļ�·��
 * @param cmd_line		�����в���
 * @param dll_path		��̬���ӿ⣨32λ��·��
 * @return				�½��̵�PID
 * @exception			EasyhookException
 */
int CreateProcessAndInject(const char* exe_path, const char* cmd_line, const char* dll_path);


} // namespace apihook
} // namespace utility