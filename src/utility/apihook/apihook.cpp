#include "apihook.h"
#include "easyhook.h.inc"
#include "../codeconv/codeconv.h"

#pragma comment(lib, "../utility/apihook/EasyHook32.lib")

namespace utility {
namespace apihook {


/**
 * 获取EasyHook的上一次异常
 */
inline EasyhookException getLastError() {
	// RtlGetLastErrorString返回的是wchar_t*，所以需要编码转化
	std::wstring error_unicode(RtlGetLastErrorString());
	std::string error_ansi;
	codeconv::unicodeToAnsi(error_unicode, error_ansi);
	return EasyhookException(error_ansi);
}


void InstallHookForThisThread(void* module, 
							  const char* function_name, 
							  void* hook_function) {
	// hook_handle指向内容需要一直存在才能使用Hook有效，所以一般不能delete
	TRACED_HOOK_HANDLE hook_handle = new HOOK_TRACE_INFO();
	u_long acl_entries[1] = { 0 };
	
	// 设置Hook
	if (LhInstallHook(GetProcAddress((HMODULE)module, function_name), hook_function, NULL, hook_handle) < 0) {
		delete hook_handle;
		throw getLastError();
	}
	// 为本线程激活此Hook
	if (LhSetInclusiveACL(acl_entries, 1, hook_handle) < 0) {
		delete hook_handle;
		throw getLastError();
	}
}

int CreateProcessAndInject(const char* exe_path, const char* cmd_line, const char* dll_path) {
	// RhCreateAndInject需要的参数是wchar_t*类型而不是char*，所以进行编码转换
	std::wstring exe_path_unicode, cmd_line_unicode, dll_path_unicode;
	codeconv::ansiToUnicode(exe_path, exe_path_unicode);
	codeconv::ansiToUnicode(cmd_line, cmd_line_unicode);
	codeconv::ansiToUnicode(dll_path, dll_path_unicode);
		
	ULONG pid;
	if (RhCreateAndInject(const_cast<wchar_t*>(exe_path_unicode.c_str()),
						  const_cast<wchar_t*>(cmd_line_unicode.c_str()),
						  CREATE_NEW_CONSOLE, 
						  EASYHOOK_INJECT_DEFAULT,
						  const_cast<wchar_t*>(dll_path_unicode.c_str()),
						  NULL,
						  NULL,
						  0,
						  &pid) < 0) throw getLastError();
	return pid;					  
}


} // namespace apihook
} // namespace utility

// ------------------------------------------------------------------------------------------

/** 
 * EasyHook远程注入DLL所需要的接口函数，在DLL加载后被调用。因为使用CreateProcessAndInject
 * 创建进程并注入，此时进程被挂起，需要在这里唤醒此进程。
 */
extern "C" 
_declspec (dllexport) 
void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* remoteInfo) {
	RhWakeUpProcess();
}