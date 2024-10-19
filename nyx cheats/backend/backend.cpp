#include "backend.hpp"

backend g_backend;

void backend::network_security() {
	if (*get_pointers()->m_is_session_started)
	{
		get_log()->send("Security", "GTA Online is not suported due to security reason go in sotry mode");
		get_hooking()->cleanup();
		get_log()->send("Security", "Hooking Disabled");
	}
}

void backend::ui() {
	get_menu()->scaled_x_coord = 0.15f;
	get_menu()->m_size = { get_renderer()->width, get_renderer()->header_height * 2 };
	get_menu()->m_position.x = 0.16f;
	get_menu()->m_position.y = 0.1175f;
	while (true) {
		if (get_menu()->is_open())
		{
			get_menu()->disable_controls();

			get_renderer()->render();
		}
		fiber::current()->sleep();
	}
}

void backend::submenu() {
	get_submenu_handler()->initialize();
	while (true) {
		if (get_menu()->is_open())
		{
			get_submenu_handler()->update(false);
		}
		fiber::current()->sleep();
	}
}

void backend::async_submenu() {
	while (true) {
		if (get_menu()->is_open())
		{
			get_submenu_handler()->async_update();
		}
		fiber::current()->sleep();
	}
}

void backend::controls() {
	while (true) {
		get_menu()->check_key();
		get_menu()->handle_key();
		if (IsOpenPressed()) {
			get_menu()->toggle_open();
		}
		fiber::current()->sleep();
	}
}

void backend::scaleform()
{
	while (true) {
		get_scaleform()->prepare_instructional_buttons();
		get_scaleform()->draw_instructional("Open Virtual X", "F5");
		get_scaleform()->draw_instructional(get_mouse()->is_enabled() ? "Mouse Off" : "Mouse On", "X");
		get_scaleform()->finish_instructional_buttons();
		fiber::current()->sleep();
	}
}

void backend::mouse() {
	while (true) {
		if (get_mouse()->is_enabled()) {
			HUD::SET_MOUSE_CURSOR_THIS_FRAME();
		}
		if (get_input()->is_key_pressed(0x58)) get_mouse()->toggle_mouse();
		get_mouse()->update_mouse();
		fiber::current()->sleep();
	}
}

void backend::discord() {
	get_discord()->initialize();
	while (true) {
		get_discord()->update();
		fiber::current()->sleep();
	}
}

void backend::register_file()
{
	std::filesystem::create_directory(g_utility.get_documents_path() + "\\Virtual X");
	std::filesystem::create_directory(g_utility.get_documents_path() + "\\Virtual X\\asi");
	std::filesystem::create_directory(g_utility.get_documents_path() + "\\Virtual X\\themes");
	std::filesystem::create_directory(g_utility.get_documents_path() + "\\Virtual X\\textures");
	std::filesystem::create_directory(g_utility.get_documents_path() + "\\Virtual X\\vehicle");
	std::filesystem::create_directory(g_utility.get_documents_path() + "\\Virtual X\\teleport");
	std::filesystem::create_directory(g_utility.get_documents_path() + "\\Virtual X\\map mods");
	std::filesystem::create_directory(g_utility.get_documents_path() + "\\Virtual X\\xml");

	auto ytd_file = g_utility.get_documents_path().append("\\Virtual X\\").append("\\textures\\").append("VX.ytd").c_str();
	get_log()->send("Files", "Downloading Files...");
	download("https://nyxcheats.xyz/backend/download/VX.ytd", ytd_file);
	get_log()->send("Files", "Downloaded.");
	static int tries = 0;
	if (tries > 5) {
		get_log()->send("Files", "Failed to download file");
		return;
	}
	static int textureID;
	//GetCore()->m_register_file(&textureID, ytd_file, true, "VX.ytd", false);
	get_log()->send("Register", std::format("VX.ytd ID: {}", textureID).c_str());

}
void backend::register_shv()
{
	if (!GetModuleHandleA("ScriptHookV.dll")) {
		auto shv_file = g_utility.get_documents_path().append("\\Virtual X\\").append("ScriptHookV.dll").c_str();
		LoadLibraryA(shv_file);
	}
	else {
		if (GetModuleHandleA("dinput8.dll")) {
			while (!GetModuleHandleA("ScriptHookV.dll")) {
				std::this_thread::sleep_for(100ms);
			}
		}
	}
	get_core()->m_script_hook_v = (uint64_t)GetModuleHandleA("ScriptHookV.dll");
	if (get_core()->m_script_hook_v) {
		MODULEINFO shvInfo = {};
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("ScriptHookV.dll"), &shvInfo, sizeof(shvInfo));
		script_hook_v::loaded_module_memory.emplace_back((uint64_t)shvInfo.lpBaseOfDll, (uint64_t)shvInfo.SizeOfImage, "ScriptHookV.dll", GetModuleHandleA("ScriptHookV.dll"));
		get_log()->send("Register", std::format("ScriptHookV.dll at : {} size : {}", (uint64_t)shvInfo.lpBaseOfDll, (uint64_t)shvInfo.SizeOfImage).c_str());
	}
	else
	{
		get_log()->send("Register", "Script Hook V Module Not Found");
	}
}

backend* get_backend() { return &g_backend; }