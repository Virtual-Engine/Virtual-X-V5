#pragma once
#include "../../common.hpp"
#include "option.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/submenu_handler.hpp"
#include "../../menu/util/menu_input.hpp"

class toggle_option : public option {
private:
	std::function<void(toggle_option*)> on_update;
	std::function<void()> function;
	std::function<void()> current_op;
	bool* bool_op;
public:

	toggle_option(const char* name) : option(name), function([] {}), on_update([](toggle_option* option) {}), bool_op(nullptr), current_op([] {}) {}

	toggle_option& add_toggle(void(*func)()) {
		function = func;
		return *this;
	}

	toggle_option& add_toggle(bool& toggle) {
		bool_op = &toggle;
		return *this;
	}

	toggle_option& add_function(std::function<void()> func) {
		function = func;
		return *this;
	}

	toggle_option& add_requirement(std::function<bool()> func) {
		Requirement = func;
		return *this;
	}

	toggle_option& add_on_update(std::function<void(toggle_option*)> update) {
		on_update = update;
		return *this;
	}

	toggle_option& add_on_update(std::function<void()> update) {
		on_update = [=](toggle_option* option) {update(); };
		return *this;
	}

	toggle_option& add_current_op(std::function<void()> func) {
		current_op = func;
		return *this;
	}

	toggle_option& add_tooltip(const char* tooltip) {
		this->tooltip = tooltip;
		return *this;
	}

	void render(int position);
	void render_selected(int position);
};
