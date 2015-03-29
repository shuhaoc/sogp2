#pragma once
#include <string>
#include <exception>

namespace utility {
namespace apihook {

/**
 * 使用EasyHook库可能发生的异常
 */
class EasyhookException : public std::exception {
public:
	EasyhookException(const std::string& message) : _message(message) { }
	
	virtual const char* what() const throw() { return _message.c_str(); }
	
private:
	std::string _message;
};


/**
 * Hook指定的函数，仅仅影响当前线程。
 * @param module			DLL模块句柄
 * @param function_name		要Hook的函数名
 * @param hook_function		Hook函数指针
 * @exception				EasyhookException
 */
void InstallHookForThisThread(void* module, const char* function_name, void* hook_function);

/**
 * 创建进程（仅限32位）并挂起，注入指定DLL文件，返回新进程的PID。
 * 注入的DLL需要有NativeInjectionEntryPoint接口，此接口在easyhook_utility.cpp中
 * 已经实现，而NativeInjectionEntryPoint函数会唤醒此进程。
 * @param exe_path		可执行文件路径
 * @param cmd_line		命令行参数
 * @param dll_path		动态链接库（32位）路径
 * @return				新进程的PID
 * @exception			EasyhookException
 */
int CreateProcessAndInject(const char* exe_path, const char* cmd_line, const char* dll_path);


} // namespace apihook
} // namespace utility