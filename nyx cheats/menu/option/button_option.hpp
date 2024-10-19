#pragma once
#include "../../common.hpp"
#include "option.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/submenu_handler.hpp"
#include "../../menu/util/menu_input.hpp"

class button_option : public option {
private:
	std::function<void(button_option*)> on_update;
	std::function<void()> function;
	std::function<void()> current_op;

	bool has_keyboard;
	bool is_keyboard_active;
	std::function<void(const char*)> keyboard_callback;
	const char* default_keyboard_text;
	int max_keyboard_chars;
public:
	button_option(const char* name) : option(name), is_keyboard_active(false), has_keyboard(false), function([] {}), on_update([](button_option* option) {}), current_op([] {}) {}

	button_option& add_function(std::function<void()> func) {
		function = func;
		return *this;
	}

	button_option& add_requirement(bool(*func)()) {
		Requirement = func;
		return *this;
	}

	button_option& addon_update(std::function<void(button_option*)> func) {
		on_update = func;
		return *this;
	}

	button_option& add_current_op(std::function<void()> func) {
		current_op = func;
		return *this;
	}

	button_option& add_tooltip(const char* tooltip) {
		this->tooltip = tooltip;
		return *this;
	}

	button_option& add_keyboard(const char* defaultText, int maxChars, std::function<void(const char*)> function) {
		has_keyboard = true;
		keyboard_callback = function;
		max_keyboard_chars = maxChars;
		default_keyboard_text = defaultText;
		return *this;
	}

	void render(int position);
	void render_selected(int position);
};
