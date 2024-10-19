#pragma once
#include "../../common.hpp"
#include <functional>

class option {
protected:
	std::function<bool()> Requirement;
	std::string name;
	std::string tooltip;
	bool is_breaks;
	bool visible;
	uint8_t hover_timer;
public:
	option() : name(""), tooltip(""), Requirement([] { return true; }), is_breaks(false), visible(true), hover_timer(0) {}
	option(const char* name) : name(name), tooltip(""), Requirement([] { return true; }), is_breaks(false), visible(true), hover_timer(0) {}
	option(const char* name, bool isBreak) : name(name), tooltip(""), Requirement([] { return true; }), is_breaks(isBreak), visible(true), hover_timer(0) {}

	option(const option& src) : name(src.name), tooltip(src.tooltip) {
		*this = src;
	}

	virtual void input(int position);
	virtual void render(int position);
	virtual void render_selected(int position);

	template<typename T>
	bool is_of_type() {
		return dynamic_cast<const T*>(this) != nullptr;
	}

	void set_visible(bool visible) { visible = visible; }

	std::string& get_name() { return name; }
	std::string& get_tooltip() { return tooltip; }

	bool is_break() { return is_breaks; }
	bool is_visible() { return visible && Requirement(); }
};
