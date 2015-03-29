#include "apihook.h"
#include "easyhook.h.inc"
#include "../codeconv/codeconv.h"

#pragma comment(lib, "../utility/apihook/EasyHook32.lib")

namespace utility {
namespace apihook {


/**
 * ��ȡEasyHook����һ���쳣
 */
inline EasyhookException getLastError() {
	// RtlGetLastErrorString���ص���wchar_t*��������Ҫ����ת��
	std::wstring error_unicode(RtlGetLastErrorString());
	std::string error_ansi;
	codeconv::unicodeToAnsi(error_unicode, error_ansi);
	return EasyhookException(error_ansi);
}


void InstallHookForThisThread(void* module, 
							  const char* function_name, 
							  void* hook_function) {
	// hook_handleָ��������Ҫһֱ���ڲ���ʹ��Hook��Ч������һ�㲻��delete
	TRACED_HOOK_HANDLE hook_handle = new HOOK_TRACE_INFO();
	u_long acl_entries[1] = { 0 };
	
	// ����Hook
	if (LhInstallHook(GetProcAddress((HMODULE)module, function_name), hook_function, NULL, hook_handle) < 0) {
		delete hook_handle;
		throw getLastError();
	}
	// Ϊ���̼߳����Hook
	if (LhSetInclusiveACL(acl_entries, 1, hook_handle) < 0) {
		delete hook_handle;
		throw getLastError();
	}
}

int CreateProcessAndInject(const char* exe_path, const char* cmd_line, const char* dll_path) {
	// RhCreateAndInject��Ҫ�Ĳ�����wchar_t*���Ͷ�����char*�����Խ��б���ת��
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
 * EasyHookԶ��ע��DLL����Ҫ�Ľӿں�������DLL���غ󱻵��á���Ϊʹ��CreateProcessAndInject
 * �������̲�ע�룬��ʱ���̱�������Ҫ�����﻽�Ѵ˽��̡�
 */
extern "C" 
_declspec (dllexport) 
void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* remoteInfo) {
	RhWakeUpProcess();
}