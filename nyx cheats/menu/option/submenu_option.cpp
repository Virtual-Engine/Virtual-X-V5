#include "submenu_option.hpp"

void submenu_option::render_selected(int position) {
	current_op();
	static timer enter_key(0ms);
	enter_key.set_delay(150ms);
	if (get_menu()->enter_key_pressed && enter_key.updated() && Requirement()) {
		function();
		get_submenu_handler()->set_submenu(submenus);
	}
};

void submenu_option::render(int position) {
	on_update(this);

	RGBA color = get_menu()->is_selected(position) ? get_renderer()->highlighted_option_color : get_renderer()->menu_text_color;
	get_renderer()->draw_string(name.c_str(), 0.742f + get_menu()->pos.x - (0.01f / 2) - 0.7f + (get_menu()->is_selected(position) ? 0.0025f : 0.f), ((position + 2) * 0.037f + 0.11f) + get_menu()->pos.y + 0.001f, get_renderer()->text_font, 0.37f, color);
	get_renderer()->draw_sprite(Texture("commonmenu", "arrowright"), (0.742f + get_menu()->pos.x - (0.01f / 2) - 0.7f) + 0.22f, ((position + 2) * 0.037f + 0.112f) + get_menu()->pos.y + 0.001f + 0.012f, 0.015f, 0.025f, 0.f, color);
}