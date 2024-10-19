#include "../../common.hpp"
#include "../../menu/util/script_hook_v.hpp"
#include "../../invoker/natives.hpp"
#include "../../utilities/input.hpp"
#include "../../fiber/fiber.hpp"
#include "../../fiber/manager.hpp"
#include "../../core/core.hpp"
#include <tlhelp32.h>
#include <set>
#include <Psapi.h>

rage::scrNativeCallContext script_hook_v::native_context;
std::vector<module_detection> script_hook_v::loaded_module_memory;
uint64_t script_hook_v::native_hash;

int script_count;
using namespace VX;

void script_hook_v::script_register(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
	static char name[256];
	if (GetModuleFileNameA(module, name, 256)) {
		if (strstr(name, "Menyoo")) {
			uint64_t base = (uint64_t)module;
			uint64_t address = (base + 0x2CDC06);
			static BYTE nops[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

			DWORD oldProtect;
			VirtualProtect((void*)address, sizeof(nops), PAGE_EXECUTE_READWRITE, &oldProtect);
			memcpy((void*)address, &nops, sizeof(nops));
			VirtualProtect((void*)address, sizeof(nops), oldProtect, &oldProtect);
		}
	}
	MODULEINFO mi = {};
	GetModuleInformation(GetCurrentProcess(), module, &mi, sizeof(MODULEINFO));
	get_log()->send("Info", "Creating Script %p and size %p", (uint64_t)mi.lpBaseOfDll, (uint64_t)mi.SizeOfImage);
	loaded_module_memory.emplace_back((uint64_t)mi.lpBaseOfDll, (uint64_t)mi.SizeOfImage, name, module);
	get_fiber_manager()->add_fiber(new fiber(std::format("{}, {}, {}", (uint64_t)module, (uint64_t)LP_SCRIPT_MAIN, script_count), LP_SCRIPT_MAIN));
	script_count += 1;
}

void script_hook_v::script_register_another_thread(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
	get_fiber_manager()->add_fiber(new fiber(std::format("{}, {}, {}", (uint64_t)module, (uint64_t)LP_SCRIPT_MAIN, script_count), LP_SCRIPT_MAIN));
	script_count += 1;
}

void script_hook_v::script_unregister(HMODULE module) {
	for (const auto& script : get_fiber_manager()->g_fibers) {
		if (strstr(script->m_name.c_str(), std::format("{}", (uint64_t)module).c_str())) {
			script->m_disabled = true;
			try {
				script->~fiber();
				FreeLibrary(module);
			}
			catch (std::exception e) {
				get_log()->send("Fibers", "deleting fiber {}", e.what());
			}
		}
	}
}

void script_hook_v::native_push(uint64_t val) {
	get_invoker()->m_call_context.push_arg(std::forward<uint64_t>(val));
}

void script_hook_v::native_call_begin(uint64_t hash) {
	get_invoker()->begin_call();
	native_hash = hash;
}


uint64_t* script_hook_v::native_call_end() {
	static uint64_t nativeBypasses[18] = {
	0x39FF19C64EF7DA5B,
	0x340E61DE7F471565,
	0xE0A7D1E497FFCD6F,
	0x2975C866E6713290,
	0xA1B043EE79A916FB,
	0x18D0456E86604654,
	0x2E397FD2ECD37C87,
	0xA4A79DD2D9600654,
	0x407C7F91DDB46C16,
	0xD24D37CC275948CC,
	0xA66C71C98D5F2CFB,
	0xFF1BED81BFDC0FE0,
	0x11879CDD803D30F4,
	0x57FFF03E423A4C0B,
	0x00A1CADD00108836,
	0x4B33C4243DE0C432,
	0x4851997F37FE9B3C,
	0xB3271D7AB655B441
	};

	bool found = false;

	for (int i = 0; i < 18; i++) {
		if (native_hash == nativeBypasses[i]) {
			found = true;
			break;
		}
	}

	if (native_hash == 0x509D5878EB39E842 || native_hash == 0x9A294B2138ABB884 || native_hash == 0x7DD959874C1FD534)
		native_context.set_arg(4, true);

	if (native_hash == 0x509D5878EB39E842) {
		native_context.set_arg(5, true);
		native_context.set_arg(6, false);
	}

	if (native_hash == 0xAF35D0D2583051B0)
		native_context.set_arg(6, false);

	if (native_hash == 0xD49F9B0955C367DE) {
		native_context.set_arg(6, true);
		native_context.set_arg(7, false);
	}

	if (native_hash == 0xD49F9B0955C367DE || native_hash == 0xAF35D0D2583051B0 || native_hash == 0x9A294B2138ABB884 || native_hash == 0x509D5878EB39E842) {
		if (!STREAMING::HAS_MODEL_LOADED(native_context.get_arg<int>(0))) {
			STREAMING::REQUEST_MODEL(native_context.get_arg<int>(0));
		}
	}

	if (native_hash == 0x39FF19C64EF7DA5B || native_hash == 0xE0A7D1E497FFCD6F || native_hash == 0x340E61DE7F471565) {
		// set wanted level?
	}

	if (native_hash == 0xA1B043EE79A916FB || native_hash == 0xA4A79DD2D9600654 || native_hash == 0x18D0456E86604654 || native_hash == 0x407C7F91DDB46C16) {
		native_context.set_arg(0, 0);
	}

	if (native_hash == 0xD24D37CC275948CC) {
		native_context.set_arg(0, MISC::GET_HASH_KEY(native_context.get_arg<char*>(0)));
	}
	get_invoker()->end_call(native_hash);
	return get_invoker()->m_call_context.get_return_value<uint64_t>();
}

uint64_t* script_hook_v::get_global(int index) {
	return (uint64_t*)&get_pointers()->m_script_globals[index >> 18 & 0x3F][index & 0x3FFFF];
}

int script_hook_v::create_texture(const char* fileName) {
	//LOG_DEBUG("[SHV] Ignoring call to createTexture");
	return 0;
}

void script_hook_v::draw_texture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a) {
	//LOG_DEBUG("[SHV] Ignoring call to drawTexture");
}

void script_hook_v::script_wait(unsigned long waitTime) {
	fiber::current()->sleep(std::chrono::milliseconds(waitTime));
}

void script_hook_v::keyboard_register(input::KeyboardFunction function) {
	get_input()->g_keyboardFunctions.insert(function);
}

void script_hook_v::keyboard_unregister(input::KeyboardFunction function) {
	get_input()->g_keyboardFunctions.erase(function);
}
int script_hook_v::get_game_verison() {
	static const bool isSteam = get_core()->m_steam;
	return isSteam ? VER_1_0_1493_1_STEAM : VER_1_0_1493_1_NOSTEAM;
}

BYTE* script_hook_v::get_script_handle(int handle) {
	auto address = get_pointers()->m_handle_to_ptr(handle);
	if (!address) return nullptr;
	return (BYTE*)address;
}

void script_hook_v::present_call_back_register(present_callback cb) {

}

void script_hook_v::present_call_back_unregister(present_callback cb) {

}

int script_hook_v::register_raw_streaming_file(const std::string& fileName, const std::string& registerAs) {
	/*int textureID = -1;
	if (get_pointers()->m_register_file(&textureID, fileName.c_str(), true, fileName.c_str(), false)) {
		g_log.send("ScriptHookV", "Registered file {} with id {}", fileName, textureID);
		return textureID;
	}*/
	return 0;
}