#pragma once
#include "../../common.hpp"
#include "option.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/submenu_handler.hpp"

class break_option : public option {
private:
	bool line;
	std::function<void(break_option*)> on_update;
	std::function<void()> current_op;

public:
	break_option() : option("________________________", true), line(true) { }
	break_option(const char* name) : option(name), on_update([](break_option* option) {}), current_op([] {}) {}

	break_option& add_requirement(bool(*func)()) {
		Requirement = func;
		return *this;
	}

	break_option& add_on_update(std::function<void(break_option*)> func) {
		on_update = func;
		return *this;
	}

	break_option& add_current_op(std::function<void()> func) {
		current_op = func;
		return *this;
	}

	break_option& get() {
		return *this;
	}

	void set_name(std::string name) {
		this->name = name;
		line = false;
	}
	void set_line() { line = true;  set_name("________________________"); }

	void input(int position);
	void render(int position);
	void render_selected(int position);
};
