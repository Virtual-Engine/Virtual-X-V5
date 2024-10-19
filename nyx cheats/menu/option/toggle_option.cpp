#include "toggle_option.hpp"

void toggle_option::render(int position) {
	on_update(this);

	RGBA color = get_menu()->is_selected(position) ? get_renderer()->highlighted_option_color : get_renderer()->menu_text_color;
	get_renderer()->draw_string(name.c_str(), 0.742f + get_menu()->pos.x - (0.01f / 2) - 0.7f + (get_menu()->is_selected(position) ? 0.0025f : 0.f), ((position + 2) * 0.037f + 0.11f) + get_menu()->pos.y + 0.001, get_renderer()->text_font, 0.37f, color);

	color = *bool_op ? RGBA(130, 214, 157, 255) : RGBA(200, 55, 80, 255);

	if (get_renderer()->toggle_style == 0) {
		if (*bool_op)
			get_renderer()->draw_sprite(Texture("commonmenu", get_menu()->is_selected(position) ? "shop_box_tickb" : "shop_box_tick"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.702f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001, 0.027f, 0.046f, 0, { 255, 255, 255, 255 });
		else
			get_renderer()->draw_sprite(Texture("commonmenu", get_menu()->is_selected(position) ? "shop_box_blankb" : "shop_box_blank"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.702f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001, 0.027f, 0.046f, 0, { 255, 255, 255, 255 });
	}
	else if (get_renderer()->toggle_style == 1) {
		if (get_menu()->is_selected(position))
			get_renderer()->draw_sprite(Texture("commonmenu", "common_medal"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.7f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001f, 0.027f, 0.046f, 0, { 0, 0, 0, 255 });

		get_renderer()->draw_sprite(Texture("commonmenu", "common_medal"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.7f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001f, 0.022f, 0.039f, 0, color);
	}
}

void toggle_option::render_selected(int position) {
	if (!Requirement())
		return;
	static timer enter_key(0ms);
	enter_key.set_delay(150ms);
	current_op();
	if (get_menu()->enter_key_pressed && enter_key.updated()) {
		if (this->bool_op != nullptr) {
			*this->bool_op = !*this->bool_op;
		}
		function();
	}
};