#pragma once
#include "../common.hpp"
#include "../invoker/natives.hpp"
#include "../utilities/utilities.hpp"
#include "../gta/enums.hpp"
#include "../utilities/timer.h"
#include "submenu.hpp"
#include "menu.hpp"

class submenu_handler
{
public:
	void initialize();
	void update(bool withInput);
	void async_update();
	void cleanup();

	std::vector<submenu*> submenus;

	void add_submenu(submenu* submenu) { submenus.push_back(submenu); }
	int get_total_options() { return total_options; }

	void set_submenu(submenu* submenu);
	void set_submenu_main();
	void set_sub_previous(bool resetPosition = false);
	submenu* get_current_submenu() { return m_current_submenu; }
	submenu* get_previous_submenu() { return m_previous_submenu; };
	std::vector<submenu*> get_submenus() { return m_submenus; }

	template<typename T>
	bool is_current_submenu_of_type() {
		return dynamic_cast<const T*>(get_current_submenu()) != nullptr;
	}

	int total_options;
	int current_option_without_breaks;
	int total_options_without_breaks;
private:
	submenu* m_main_submenu;
	submenu* m_current_submenu;
	submenu* m_previous_submenu;
	submenu* m_next_submenu;

	int m_next_current_option;
	int m_next_scroll_offset;

	std::vector<submenu*> m_submenus;
};

submenu_handler* get_submenu_handler();