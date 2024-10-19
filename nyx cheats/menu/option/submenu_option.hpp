#pragma once
#include "../../common.hpp"
#include "option.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/util/menu_input.hpp"

class submenu;

class submenu_option : public option {
private:
	std::function<void(submenu_option*)> on_update;
	std::function<void()> current_op;
	std::function<void()> function;

	submenu* submenus;
public:
	submenu_option(const char* name) : option(name), function([] {}), on_update([](submenu_option* option) {}), submenus(nullptr), current_op([] {}) {}

	template<typename T, typename = std::enable_if<std::is_base_of<submenu, T>::value>>
	submenu_option& add_submenu() {
		submenus = T::get_instance();
		return *this;
	}

	submenu_option& add_submenu(submenu* submenu) {
		submenus = submenu;
		return *this;
	}

	submenu_option& add_function(std::function<void()> func) {
		function = func;
		return *this;
	}

	submenu_option& add_requirement(std::function<bool()> func) {
		Requirement = func;
		return *this;
	}

	submenu_option& add_on_update(std::function<void(submenu_option*)> func) {
		on_update = func;
		return *this;
	}

	submenu_option& add_current_op(std::function<void()> func) {
		current_op = func;
		return *this;
	}

	submenu_option& set_visible(bool visible) {
		visible = visible;
		return *this;
	}

	submenu_option& add_name(const char* name) {
		this->name = name;
		return *this;
	}

	submenu_option& add_tooltip(const char* tooltip) {
		this->tooltip = tooltip;
		return *this;
	}

	void render(int position);
	void render_selected(int position);
};