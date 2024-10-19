#pragma once
#include "option.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/submenu_handler.hpp"
#include "../../menu/util/menu_input.hpp"
#include "../../utilities/utilities.hpp"

typedef enum {
	SCROLL,
	SELECT,
	TOGGLE, 
	SCROLLSELECT
}ScrollOptionType;

template <typename Type>
class number_option : public option {
private:
	std::function<void(number_option*)> on_update;
	std::function<void()> current_op;
	std::function<void()> function;

	ScrollOptionType scroll_type;
	const char* Format;
	Type* number_op;
	bool* bool_op;
	Type step;
	Type min;
	Type max;

	std::shared_ptr<int> selected_number_storage;
	int* selected_number_op;

	DWORD64 scroll_speed_op;
	bool has_min;
	bool has_max;
	bool loop_op;

public:
	number_option(ScrollOptionType type, const char* name) : option(name), scroll_type(type), has_min(false), has_max(false), loop_op(false), scroll_speed_op(100), function([] {}), on_update([](number_option* option) {}), current_op([] {}) {}

	number_option& add_toggle(bool& toggle) {
		bool_op = &toggle;
		return *this;
	}

	number_option& add_number(Type& number, const char* format, Type step) {
		if (scroll_type == SELECT) {
			selected_number_storage = std::make_shared<int>(0);
			selected_number_op = selected_number_storage.get();
		}

		number_op = &number;
		Format = format;
		step = step;
		return *this;
	}

	number_option& add_selected_number(int& selectedNumber) {
		selected_number_op = &selectedNumber;
		return *this;
	}

	number_option& add_min(Type min) {
		min = min;
		has_min = true;
		return *this;
	}

	number_option& add_max(Type max) {
		max = max;
		has_max = true;
		return *this;
	}

	number_option& add_requirement(std::function<bool()> func) {
		Requirement = func;
		return *this;
	}

	number_option& add_function(std::function<void()> func) {
		function = func;
		return *this;
	}

	number_option& addon_update(std::function<void()> update) {
		on_update = [=](number_option* option) { update(); };
		return *this;
	}

	number_option& add_on_update(std::function<void(number_option* option)> update) {
		on_update = update;
		return *this;
	}

	number_option& add_current_op(std::function<void()> func) {
		current_op = func;
		return *this;
	}

	number_option& set_scroll_speed(int speed) {
		scroll_speed_op = speed;
		return *this;
	}

	number_option& add_tooltip(const char* tooltip) {
		this->tooltip = tooltip;
		return *this;
	}

	number_option& can_loop() {
		loop_op = true;
		return *this;
	}

	void render(int position);
	void render_selected(int position);
};
