#pragma once
#include "../../submenu.hpp"
#include "../../submenu_handler.hpp"
#include "../../option/submenu_option.hpp"
#include "../../option/button_option.hpp"
#include "../../option/break_option.hpp"
#include "../../option/toggle_option.hpp"

class search_menu : public submenu
{
public:
	static search_menu* get_instance();

	void initialize() override;
	void update_once() override;
	void update() override;
	void feature_update() override;

	search_menu() : submenu() { }
	~search_menu();
};

namespace search_menu_vars
{
	struct var {
	};
}