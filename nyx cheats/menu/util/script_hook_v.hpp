#pragma once
#include "../../common.hpp"

typedef void(*keyboard_handler)(DWORD, WORD, BYTE, BOOL, BOOL, BOOL, BOOL);
typedef void(*present_callback)(void*);

enum eGameVersion : int
{
	VER_1_0_335_2_STEAM,
	VER_1_0_335_2_NOSTEAM,

	VER_1_0_350_1_STEAM,
	VER_1_0_350_2_NOSTEAM,

	VER_1_0_372_2_STEAM,
	VER_1_0_372_2_NOSTEAM,

	VER_1_0_393_2_STEAM,
	VER_1_0_393_2_NOSTEAM,

	VER_1_0_393_4_STEAM,
	VER_1_0_393_4_NOSTEAM,

	VER_1_0_463_1_STEAM,
	VER_1_0_463_1_NOSTEAM,

	VER_1_0_505_2_STEAM,
	VER_1_0_505_2_NOSTEAM,

	VER_1_0_573_1_STEAM,
	VER_1_0_573_1_NOSTEAM,

	VER_1_0_617_1_STEAM,
	VER_1_0_617_1_NOSTEAM,

	VER_1_0_678_1_STEAM,
	VER_1_0_678_1_NOSTEAM,

	VER_1_0_757_2_STEAM,
	VER_1_0_757_2_NOSTEAM,

	VER_1_0_757_4_STEAM,
	VER_1_0_757_4_NOSTEAM,

	VER_1_0_791_2_STEAM,
	VER_1_0_791_2_NOSTEAM,

	VER_1_0_877_1_STEAM,
	VER_1_0_877_1_NOSTEAM,

	VER_1_0_944_2_STEAM,
	VER_1_0_944_2_NOSTEAM,

	VER_1_0_1011_1_STEAM,
	VER_1_0_1011_1_NOSTEAM,

	VER_1_0_1032_1_STEAM,
	VER_1_0_1032_1_NOSTEAM,

	VER_1_0_1103_2_STEAM,
	VER_1_0_1103_2_NOSTEAM,

	VER_1_0_1180_2_STEAM,
	VER_1_0_1180_2_NOSTEAM,

	VER_1_0_1290_1_STEAM,
	VER_1_0_1290_1_NOSTEAM,

	VER_1_0_1365_1_STEAM,
	VER_1_0_1365_1_NOSTEAM,

	VER_1_0_1493_0_STEAM,
	VER_1_0_1493_0_NOSTEAM,

	VER_1_0_1493_1_STEAM,
	VER_1_0_1493_1_NOSTEAM,

	VER_SIZE,
	VER_UNK = -1
};

struct module_detection {
	uint64_t m_base;
	uint64_t m_size;
	HMODULE m_module;
	char m_szName[100];

	module_detection() {}

	module_detection(uint64_t a, uint64_t b, const char* c, HMODULE hm)
		: m_base(a), m_size(b), m_module(hm) {
		strcpy(m_szName, c);
	}

	static friend bool operator==(const module_detection& left, HMODULE module) {
		return left.m_module == module;
	}
};

class script_hook_v {
public:
	static rage::scrNativeCallContext native_context;
	static uint64_t native_hash;
	static std::vector<module_detection> loaded_module_memory;

	static void script_register(HMODULE module, void(*LP_SCRIPT_MAIN)());
	static void script_register_another_thread(HMODULE module, void(*LP_SCRIPT_MAIN)());
	static void script_unregister(HMODULE module);

	static BYTE* get_script_handle(int handle);
	static void present_call_back_register(present_callback cb);
	static void present_call_back_unregister(present_callback cb);

	static void native_call_begin(uint64_t hash);
	static void native_push(uint64_t val);
	static uint64_t* native_call_end();

	static uint64_t* get_global(int index);

	static int create_texture(const char* fileName);
	static void draw_texture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a);

	static void keyboard_register(keyboard_handler function);
	static void keyboard_unregister(keyboard_handler function);

	static void script_wait(unsigned long waitTime);
	static void wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//static int world_get_all_vehicles(int* arr, int arrSize);
	//static int world_get_all_peds(int* arr, int arrSize);
	//static int world_get_all_objects(int* arr, int arrSize);
	//static int world_get_all_pickups(int* arr, int arrSize);

	static int get_game_verison();

	static int register_raw_streaming_file(const std::string& fileName, const std::string& registerAs);
};