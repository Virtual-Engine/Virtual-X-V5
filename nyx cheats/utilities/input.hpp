#pragma once
#include "../common.hpp"
#include "log/log.hpp"
#include <set>
class input {
public:
	typedef void(*KeyboardFunction)(DWORD key, WORD repeats, uint8_t scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
	bool initialize();
	void cleanup();

	bool is_key_pressed(const int virtual_key);

	inline static std::set<KeyboardFunction> g_keyboardFunctions;
private:
	static LRESULT APIENTRY window_process(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	inline static WNDPROC m_windowProc;
	HWND m_hwnd{};
};

input* get_input();