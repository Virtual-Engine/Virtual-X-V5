#include "search.hpp"
#include "../../submenus/home.hpp"
#include "../../util/script_hook_v.hpp"

using namespace search_menu_vars;

namespace search_menu_vars {
	var vars;
}

void search_menu::initialize() {
	set_name("Search", false);
	set_parent_submenu<main_menu>();
}

void search_menu::update_once() {
}

void search_menu::update() {
}

void search_menu::feature_update() {}

search_menu* _instance;
search_menu* search_menu::get_instance() {
	if (_instance == nullptr) {
		_instance = new search_menu();
		_instance->initialize();
		get_submenu_handler()->add_submenu(_instance);
	}
	return _instance;
}
search_menu::~search_menu() { delete _instance; }