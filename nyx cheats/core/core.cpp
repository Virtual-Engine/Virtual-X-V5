#include "core.hpp"
#include "../hooking/hooking.hpp"
#include "../invoker/invoker.hpp"
#include "../common.hpp"
#include "../menu/util/script_hook_v.hpp"
#include <memory>
#include "../pointers/pointers.hpp"
#include "../utilities/input.hpp"
#include "../utilities/utilities.hpp"
#include "../menu/menu.hpp"
#include "../menu/texture/direct_x.hpp"
#include "../menu/util/mouse.hpp"
#include "../utilities/scaleform.h"
#include "../menu/util/menu_input.hpp"
#include "../utilities/log/log.hpp"
#include "../backend/backend.hpp"
#include <Psapi.h>

namespace VX {

	core g_core;
	void core::initialize() {
		get_core()->m_steam = (uint64_t)GetModuleHandleA("steam_api64.dll") > 0;
		get_log()->initiliaze();
		get_pointers()->initialize();
		get_hooking()->initialize();
		get_invoker()->initialize();
		get_input()->initialize();
		get_direct_x()->initialize();

		get_fiber_manager()->add_fiber(new fiber("UI", &backend::ui));
		get_fiber_manager()->add_fiber(new fiber("Submenu", &backend::submenu));
		get_fiber_manager()->add_fiber(new fiber("Async Submenu", &backend::async_submenu));
		get_fiber_manager()->add_fiber(new fiber("Controls", &backend::controls));
		get_fiber_manager()->add_fiber(new fiber("Mouse", &backend::mouse));
		get_fiber_manager()->add_fiber(new fiber("Scaleform", &backend::scaleform));
		get_fiber_manager()->add_fiber(new fiber("Network Security", &backend::network_security));
		get_fiber_manager()->add_fiber(new fiber("Discord", &backend::discord));
		get_fiber_manager()->add_fiber(new fiber("Register Files", &backend::register_file));
		get_fiber_manager()->add_fiber(new fiber("Register ScriptHookV", &backend::register_shv));

		get_log()->send("Service", "Virtual X Loaded");
	}

	void core::cleanup() {
		std::this_thread::sleep_for(500ms);
		get_log()->cleanup();
		get_hooking()->cleanup();
		get_input()->cleanup();
		get_submenu_handler()->cleanup();
		get_discord()->cleanup();
		get_fiber_manager()->cleanup();
	}

	core* get_core() { return &g_core; };
}
