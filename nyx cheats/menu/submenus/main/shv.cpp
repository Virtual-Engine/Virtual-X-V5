#include "shv.hpp"
#include "../../submenus/home.hpp"
#include "../../util/script_hook_v.hpp"

using namespace shv_menu_vars;

namespace shv_menu_vars {
	var vars;

	std::map<std::string, HMODULE> m_plugin_handles;
	void load_asi(std::string script) {
		auto lib = LoadLibraryA(g_utility.get_documents_path().append("\\Virtual X\\").append("asi\\").append(script).c_str());
		m_plugin_handles.insert({ script, lib });
	}
}

void shv_menu::initialize() {
	set_name("ScriptHookV", true);
	set_parent_submenu<main_menu>();

	add_option(button_option("Unload All").add_function([] {
		for (auto plugin : m_plugin_handles) {
			script_hook_v::script_unregister(plugin.second);
		}})
		.add_tooltip("Unload All Asi"));

	add_option(break_option().get());
}

void shv_menu::update_once() {
	clear_options_offset(2);
	auto filePath = std::filesystem::path(g_utility.get_documents_path().append("\\Virtual X\\").append("asi"));
	std::filesystem::directory_iterator dirIt{ filePath.string() };
	static std::string hdrName{};
	for (auto&& dirEntry : dirIt) {
		if (dirEntry.is_regular_file()) {
			auto path = dirEntry.path();
			if (path.has_filename()) {
				if (path.extension().string() == ".asi") {
					add_option(button_option(path.filename().string().c_str()).add_function([&] {
							load_asi(path.filename().string());
						})
						.add_tooltip(path.filename().string().c_str()));
				}
			}
		}
	}
}

void shv_menu::update() {
}

void shv_menu::feature_update() {}

shv_menu* _instance;
shv_menu* shv_menu::get_instance() {
	if (_instance == nullptr) {
		_instance = new shv_menu();
		_instance->initialize();
		get_submenu_handler()->add_submenu(_instance);
	}
	return _instance;
}
shv_menu::~shv_menu() { delete _instance; }