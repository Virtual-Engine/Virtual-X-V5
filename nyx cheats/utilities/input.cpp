#include "../common.hpp"
#include "input.hpp"
#include "../utilities/utilities.hpp"
#include "../menu/util/keyboard.hpp"
#include <set>
using namespace VX;

input g_input;

bool input::initialize() {
	m_hwnd = FindWindowW(L"grcWindow", nullptr);
	if (!m_hwnd)
		throw std::runtime_error("Failed to find the game's window.");

	m_windowProc = (WNDPROC)SetWindowLongPtrA(m_hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)window_process);

	if (m_windowProc == nullptr) return false;
	get_log()->send("Service", "IH initialized");
	return true;
}

LRESULT APIENTRY input::window_process(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_SIZE: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	case WM_LBUTTONDOWN: break;
	case WM_LBUTTONUP: break;
	case WM_RBUTTONDOWN: break;
	case WM_RBUTTONUP: break;
	case WM_MBUTTONDOWN: break;
	case WM_MBUTTONUP: break;
	case WM_MOUSEWHEEL: break;
	case WM_MOUSEMOVE: break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	{
		get_keyboard()->on_keyboard_message((DWORD)wParam, lParam & 0xFFFF, (lParam >> 16) & 0xFF, (lParam >> 24) & 1, (uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP), (lParam >> 30) & 1, (uMsg == WM_SYSKEYUP || uMsg == WM_KEYUP));
		auto functions = g_keyboardFunctions;
		for (auto& function : functions)
			function((DWORD)wParam, lParam & 0xFFFF, (lParam >> 16) & 0xFF, (lParam >> 24) & 1, (uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP), (lParam >> 30) & 1, (uMsg == WM_SYSKEYUP || uMsg == WM_KEYUP));
		break;
	}
	}
	return CallWindowProc(m_windowProc, hwnd, uMsg, wParam, lParam);
}

bool input::is_key_pressed(const int virtual_key) {
	if (GetForegroundWindow() == m_hwnd) {
		if (GetAsyncKeyState(virtual_key) & 0x8000) {
			return true;
		}
	}
	return false;
}

void input::cleanup() {
	SetWindowLongPtrA(m_hwnd, GWLP_WNDPROC, (LONG_PTR)m_windowProc);
	get_log()->send("Service", "Disabled IH");
}

input* get_input() { return &g_input; }