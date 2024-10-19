#pragma once
#include <Windows.h>
#include "../pointers/pointers.hpp"
#include "../../lib/include/libcurl/include/curl.h"

#include "../discord/backend/handler.h"

#include "../fiber/fiber.hpp"
#include "../fiber/manager.hpp"
#include "../fiber/queue.h"

namespace VX {

	class core {
	public:
		void initialize();
		void cleanup();
	public:
		HMODULE m_module{};
		HANDLE m_main_thread;
		bool m_steam;
		DWORD m_main_thread_id;
		uint64_t request_streaming_file;
		bool m_script_hook_v = false;

	public:
		void get_loading_screen()
		{
			if (*get_pointers()->m_loading_screen_state != rage::eLoadingScreenState::Finished)
			{
				while (*get_pointers()->m_loading_screen_state != rage::eLoadingScreenState::Finished)
				{
					std::this_thread::sleep_for(1000ms);
				}
			}
		}
	};
	core* get_core();

	static HWND m_hWindow;

	inline static std::size_t WriteData(void* ptr, std::size_t size, std::size_t nmemb, void* stream)
	{
		auto written = fwrite(ptr, size, nmemb, (FILE*)stream);
		return written;
	}
	inline void download(std::string url, std::string path)
	{
		CURL* curl;
		FILE* fp;
		CURLcode res;
		curl = curl_easy_init();
		if (curl)
		{
			fp = fopen(path.c_str(), "wb");
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			fclose(fp);
		}
	}
	inline bool file_exists(const std::filesystem::path& filename) {
		const std::filesystem::path p = filename;
		return (std::filesystem::exists(p));
	}
};