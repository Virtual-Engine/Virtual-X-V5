#pragma once
#include "../common.hpp"
#include "../invoker/natives.hpp"
#include "../utilities/utilities.hpp"
#include "../gta/enums.hpp"
#include "../utilities/timer.h"
#include "../menu/option/option.hpp"

class submenu
{

private:
	std::string name;
	submenu* parent_submenu;
public:
	bool can_be_searched;
	int old_current_option;
	int old_scroll_offset;
public:
	void menu_update(bool withInput);

	virtual ~submenu();
	submenu() : name(""), parent_submenu(nullptr), can_be_searched(false), old_current_option(0), old_scroll_offset(0) {}

	virtual void initialize();
	virtual void update_once();
	virtual void update();
	virtual void feature_update();

	std::vector<std::shared_ptr<option>> get_options() { return options; }
	std::vector<std::shared_ptr<option>> options;

	template<typename T, typename = std::enable_if<std::is_base_of<option, T>::value>>
	std::shared_ptr<T> add_option(T& option) {
		std::shared_ptr<T> ptr = std::make_shared<T>(option);
		options.push_back(ptr);
		return ptr;
	}

	template<typename T, typename = std::enable_if<std::is_base_of<option, T>::value>>
	std::shared_ptr<T>& add_option(std::shared_ptr<T>& option) {
		options.push_back(option);
		return option;
	}

	template<typename T, typename = std::enable_if<std::is_base_of<submenu, T>::value>>
	void set_parent_submenu() {
		parent_submenu = T::get_instance();
	}

	/*void SetParentSubmenu(Submenu*& sub) {
		pParentSubmenu = sub;
	}*/

	submenu* get_parent_submenu() { return parent_submenu; }

	void set_name(const char* name, bool canBeSearched) {
		this->name = name;
		this->can_be_searched = canBeSearched;
	}

	std::string& get_name() { return name; }

	void clear_options() {
		options.clear();
	}

	void clear_options_offset(int offset) {
		if (offset > options.size()) return;
		options.resize(offset);
	}
};