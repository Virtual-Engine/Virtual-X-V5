#include "../../common.hpp"
#include "../../hooking/hooking.hpp"
#include "../../menu/util/script_hook_v.hpp"
namespace VX
{
	FARPROC hooks::get_proc_address(HMODULE hModule, const char* lpProcName) {
		if ((unsigned int)hModule == 0x46b5ab27) { // cockstar
			if (strstr(lpProcName, "native_push")) {
				return (FARPROC)&script_hook_v::native_push;
			}
			else if (strstr(lpProcName, "create_texture")) {
				return (FARPROC)&script_hook_v::create_texture;
			}
			else if (strstr(lpProcName, "draw_texture")) {
				return (FARPROC)&script_hook_v::draw_texture;
			}
			else if (strstr(lpProcName, "get_global_ptr")) {
				return (FARPROC)&script_hook_v::get_global;
			}
			else if (strstr(lpProcName, "script_register")) {
				return (FARPROC)&script_hook_v::script_register;
			}
			else if (strstr(lpProcName, "keyboard_handler_register")) {
				return (FARPROC)&script_hook_v::keyboard_register;
			}
			else if (strstr(lpProcName, "script_register_another_thread")) {
				return (FARPROC)&script_hook_v::script_register_another_thread;
			}
			else if (strstr(lpProcName, "keyboard_handler_unregister")) {
				return (FARPROC)&script_hook_v::keyboard_unregister;
			}
			else if (strstr(lpProcName, "script_unregister")) {
				return (FARPROC)&script_hook_v::script_unregister;
			}
			else if (strstr(lpProcName, "native_call_begin")) {
				return (FARPROC)&script_hook_v::native_call_begin;
			}
			else if (strstr(lpProcName, "get_game_verison")) {
				return (FARPROC)&script_hook_v::get_game_verison;
			}
			else if (strstr(lpProcName, "script_wait")) {
				return (FARPROC)&script_hook_v::script_wait;
			}
			else if (strstr(lpProcName, "native_call_end")) {
				return (FARPROC)&script_hook_v::native_call_end;
			}
			else if (strstr(lpProcName, "get_script_handle")) {
				return (FARPROC)&script_hook_v::get_script_handle;
			}
			else if (strstr(lpProcName, "present_call_back_register")) {
				return (FARPROC)&script_hook_v::present_call_back_register;
			}
			else if (strstr(lpProcName, "present_call_back_unregister")) {
				return (FARPROC)&script_hook_v::present_call_back_unregister;
			}
			else if (strstr(lpProcName, "register_raw_streaming_file")) {
				return (FARPROC)&script_hook_v::register_raw_streaming_file;
			}
		}
		return get_hooking()->m_get_proc_adress.get_original<decltype(&hooks::get_proc_address)>()(hModule, lpProcName);
	}
}