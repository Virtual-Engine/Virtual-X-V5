#pragma once
#include "../../submenu.hpp"
#include "../../submenu_handler.hpp"
#include "../../option/submenu_option.hpp"
#include "../../option/button_option.hpp"
#include "../../option/break_option.hpp"
#include "../../option/toggle_option.hpp"
#include "../../option/number_option.hpp"

class self_menu : public submenu
{
public:
	static self_menu* get_instance();

	void initialize() override;
	void update_once() override;
	void update() override;
	void feature_update() override;

	self_menu() : submenu() { }
	~self_menu();
};

namespace self_menu_vars
{
	struct var {
		bool m_godmod = false;
		bool m_invisible = false;
		bool m_spin_bot = false;
		int m_opacity = 255;
		bool m_level_wanted = false;
		int m_level_number = 0;
		bool m_never_wanted = false;
		bool m_fake_wanted = false;
		int m_fake_wanted_number = 0;
	};
}