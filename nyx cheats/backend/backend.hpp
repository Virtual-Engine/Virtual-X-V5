#pragma once
#include "../common.hpp"
#include "../utilities/input.hpp"
#include "../utilities/utilities.hpp"
#include "../menu/menu.hpp"
#include "../menu/texture/direct_x.hpp"
#include "../menu/util/mouse.hpp"
#include "../utilities/scaleform.h"
#include "../menu/util/menu_input.hpp"
#include "../utilities/log/log.hpp"
#include "../core/core.hpp"
#include "../hooking/hooking.hpp"
#include "../invoker/invoker.hpp"
#include "../menu/util/script_hook_v.hpp"
#include "../discord/backend/handler.h"

#include <Psapi.h>

class backend
{
public:
	static void network_security();
	static void register_file();
	static void register_shv();
	static void discord();
	static void submenu();
	static void async_submenu();
	static void mouse();
	static void scaleform();
	static void controls();
	static void ui();
};

backend* get_backend();