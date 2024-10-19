#include "submenu.hpp"
#include "renderer.hpp"
#include "menu.hpp"
#include "submenu_handler.hpp"
#include "option/option.hpp"

void submenu::menu_update(bool withInput) {
	update();
	get_renderer()->draw_title(submenu::name.c_str());
	get_submenu_handler()->current_option_without_breaks = 0;
	get_submenu_handler()->total_options_without_breaks = 0;
	get_submenu_handler()->total_options = 0;
	std::vector<std::shared_ptr<option>> tempOptions;
	tempOptions.reserve(options.size());
	if (options.size() > 0) {
		int relativeOptionPosition = 0, visibilityOffset = 0;
		for (int i = 0; i < options.size(); i++) {
			if (options.at(i)->is_visible()) {
				tempOptions.push_back(options.at(i));
				visibilityOffset++;
				get_submenu_handler()->total_options++;
				if (!options.at(i)->is_break()) {
					if (visibilityOffset <= get_menu()->m_current_option) get_submenu_handler()->current_option_without_breaks++;
					get_submenu_handler()->total_options_without_breaks++;
				}
			}
		}
		for (int i = get_menu()->m_scroll_offset; i < get_menu()->m_scroll_offset + get_menu()->m_max_options; i++) {
			if (i >= 0 && i < tempOptions.size()) {
				if (relativeOptionPosition >= get_menu()->m_max_options) break;
				tempOptions.at(i).get()->render(relativeOptionPosition);
				if (get_menu()->is_open() && withInput) tempOptions.at(i).get()->input(relativeOptionPosition);
				if (relativeOptionPosition == get_menu()->m_current_option - get_menu()->m_scroll_offset) {
					option* option = tempOptions.at(i).get();
					get_renderer()->render_tooltip(option->get_tooltip());
					//	tempOptions.at(i).get()->RenderSelected(relativeOptionPosition, m_ParentNameStack);
					if (get_menu()->is_open()) option->render_selected(relativeOptionPosition);
				} relativeOptionPosition++;
			}
		}
	}
}
submenu::~submenu() {}
void submenu::initialize() {}
void submenu::update_once() {}
void submenu::update() {}
void submenu::feature_update() {}