#define _CRT_SECURE_NO_WARNINGS
#include "log.hpp"
namespace VX {
	log g_log;

	void log::initiliaze() {
		init_console_handles();
		init_files_handles();
		get_log()->send("Service", "Console Allocated");
		get_log()->send("Service", "Version -> {}", VERSION);
	}

	void log::cleanup() {
		get_log()->send("Service", "You can now close this console...");
		FreeConsole();
		PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
	}

	void log::init_console_handles() {
		AttachConsole(GetCurrentProcessId());
		AllocConsole();
		SetConsoleTitleA(m_menu_name);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
		std::this_thread::sleep_for(40ms);
		m_console_hwnd = FindWindowA(NULL, m_menu_name);
		SetWindowPos(m_console_hwnd, m_console_hwnd, 0, 0, 600, 480, SWP_FRAMECHANGED | WS_VISIBLE);
	}
	void log::init_files_handles() {
		g_log.m_console.open("CONOUT$");
		try {
			g_log.m_path.append(g_utility.get_documents_path());
			g_log.m_path.append(m_menu_name);
			if (!std::filesystem::exists(g_log.m_path))
				std::filesystem::create_directory(g_log.m_path);
			g_log.m_path.append(std::format("{}.{}", m_menu_name, "log"));
		}
		catch (std::filesystem::filesystem_error const& err) {
			g_log.send("std::filesystem error %s\n", err.what());
		}
		g_log.m_file.open(g_log.m_path, std::ios_base::out | std::ios_base::app);
		g_log.m_file.clear();
	}

	void log::free_console_handles() {
		fclose(stdout);
		FreeConsole();
	}

	void log::free_file_handles() {
		m_console.clear();
		m_path.clear();
		m_console.close();
		m_file.close();
	}

	log* get_log() { return &g_log; }
}