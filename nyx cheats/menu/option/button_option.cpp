#include "button_option.hpp"

void button_option::render(int position) {
	on_update(this);
	RGBA color = get_menu()->is_selected(position) ? get_renderer()->highlighted_option_color : get_renderer()->menu_text_color;
	get_renderer()->draw_string(name.c_str(), 0.742f + get_menu()->pos.x - (0.01f / 2) - 0.7f + (get_menu()->is_selected(position) ? 0.0025f : 0.f), ((position + 2) * 0.037f + 0.11f) + get_menu()->pos.y + 0.001f, get_renderer()->text_font, 0.37f, color);
}

void button_option::render_selected(int position) {
	current_op();
	if (get_menu()->enter_key_pressed && Requirement()) {
		function();

		if (has_keyboard) {
			is_keyboard_active = true;
			MISC::DISPLAY_ONSCREEN_KEYBOARD(0, "KeyboardManager", "", default_keyboard_text, "", "", "", max_keyboard_chars);
		}
	}

	if (is_keyboard_active) {
		if (MISC::UPDATE_ONSCREEN_KEYBOARD() > 1) {
			is_keyboard_active = false;
		}
		else {
			if (MISC::UPDATE_ONSCREEN_KEYBOARD() == 1) {
				is_keyboard_active = false;
				keyboard_callback(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
			}
		}
	}
}