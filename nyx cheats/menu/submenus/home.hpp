#pragma once
#include "../submenu.hpp"
#include "../submenu_handler.hpp"
#include "../option/submenu_option.hpp"

class main_menu : public submenu
{
public:
	static main_menu* get_instance();

	void initialize() override;
	void update_once() override;
	void update() override;
	void feature_update() override;

	main_menu() : submenu() { }
	~main_menu();
};

namespace main_menu_vars
{
	struct var {
		std::string result = "";
	};
}