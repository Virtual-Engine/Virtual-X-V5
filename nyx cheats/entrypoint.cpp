#include "core/core.hpp"

BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID) {
	using namespace VX;

	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hInstance);
		get_core()->m_module = hInstance;
		get_core()->m_main_thread = CreateThread(
			nullptr,
			0,
			[](LPVOID) -> DWORD {
				get_core()->initialize();
				while (g_running) {
					if (GetAsyncKeyState(VK_F7) & 0x8000) {
						g_running = false;
					}
					std::this_thread::sleep_for(1ms);
				}
				get_core()->cleanup();
				CloseHandle(get_core()->m_main_thread);
				FreeLibraryAndExitThread(get_core()->m_module, 0);
			}, nullptr,
			0,
			&get_core()->m_main_thread_id);
	}
	return true;
}
