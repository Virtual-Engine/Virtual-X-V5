#include "home.hpp"
#include "main/self.hpp"
#include "main/shv.hpp"
#include "main/search.hpp"

using namespace main_menu_vars;

namespace main_menu_vars {
	var vars;
	void search_options(const std::string& str) {
		search_menu::get_instance()->clear_options();
		bool foundSomething = false;
		for (auto& sub : get_submenu_handler()->get_submenus()) {
			if (!sub->can_be_searched) continue;
			std::vector<std::shared_ptr<option>> tempOptions;
			for (auto& op : sub->get_options()) {
				if (op->is_visible()) {
					std::string opName = g_math.toLower(op->get_name().c_str());
					if (opName.find(str) != std::string::npos) {
						if (!op->is_break()) {
							tempOptions.push_back(op);
							foundSomething = true;
						}
					}
				}
			}
			if (tempOptions.size() > 0) {
				search_menu::get_instance()->add_option(break_option(sub->get_name().c_str()).get());
				for (auto& op : tempOptions) {
					search_menu::get_instance()->add_option(op);
				}
			}
		}
		if (foundSomething) get_submenu_handler()->set_submenu(search_menu::get_instance());
		else get_log()->send("Info", "No result found");
	}
}

void main_menu::initialize() {
	set_name("Home", true);

	this->add_option(submenu_option("Self")
		.add_submenu<self_menu>()
		.add_tooltip("Option for your self"));

	this->add_option(submenu_option("Player")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("Vehicle")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("Weapon")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("Misc")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("Protection")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("Teleport")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("Network")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("Recovery")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("ScriptHookV")
		.add_submenu<shv_menu>());

	this->add_option(submenu_option("Spawner")
		.add_submenu<self_menu>());

	this->add_option(submenu_option("Settings")
		.add_submenu<self_menu>());

	this->add_option(button_option("Search").add_function([] {
		PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Search:", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 32);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			fiber::current()->sleep();
		}
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) {
			return;
		}
		vars.result = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
		search_options(g_math.toLower(vars.result));
		}).add_tooltip("Search option"));
}

void main_menu::update_once() {

}

void main_menu::update() {
}

void main_menu::feature_update() {}

/*Singleton*/
main_menu* _instance;
main_menu* main_menu::get_instance() {
	if (_instance == nullptr) {
		_instance = new main_menu();
		_instance->initialize();
		get_submenu_handler()->add_submenu(_instance);
	}
	return _instance;
}
main_menu::~main_menu() { delete _instance; }