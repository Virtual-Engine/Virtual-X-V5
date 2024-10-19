#pragma once
#include "../../submenu.hpp"
#include "../../submenu_handler.hpp"
#include "../../option/submenu_option.hpp"
#include "../../option/button_option.hpp"
#include "../../option/break_option.hpp"
#include "../../option/toggle_option.hpp"

class shv_menu : public submenu
{
public:
	static shv_menu* get_instance();

	void initialize() override;
	void update_once() override;
	void update() override;
	void feature_update() override;

	shv_menu() : submenu() { }
	~shv_menu();
};

namespace shv_menu_vars
{
	struct var {
	};
}