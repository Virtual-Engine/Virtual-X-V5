#include "number_option.hpp"

template <class Type>
void number_option<Type>::render(int position) {
	on_update(this);
	bool current = get_menu()->is_selected(position);

	RGBA textColor = current ? get_renderer()->highlighted_option_color : get_renderer()->menu_text_color;

	RGBA color;
	switch (scroll_type) {
	case TOGGLE:
		get_renderer()->draw_string(g_math.cprintf(Format, *number_op), get_menu()->scaled_x_coord + 0.658f + get_menu()->pos.x + (0.01f / 2) - 0.7f, ((position + 2) * 0.037f + 0.11f) + get_menu()->pos.y + 0.001f, get_renderer()->text_font, 0.37f, textColor, false, 2, current ? 0.727f : 0.711f);

		color = *bool_op ? RGBA(130, 214, 157, 255) : RGBA(200, 55, 80, 255);
		if (get_renderer()->toggle_style == 0) {
			if (*bool_op)
				get_renderer()->draw_sprite(Texture("commonmenu", get_menu()->is_selected(position) ? "shop_box_tickb" : "shop_box_tick"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.7f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001f, 0.022f, 0.039f, 0, { 255, 255, 255, 255 });
			else
				get_renderer()->draw_sprite(Texture("commonmenu", get_menu()->is_selected(position) ? "shop_box_blankb" : "shop_box_blank"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.7f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001f, 0.022f, 0.039f, 0, { 255, 255, 255, 255 });
		}
		else if (get_renderer()->toggle_style == 1) {
			get_renderer()->draw_sprite(Texture("commonmenu", "common_medal"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.7f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001f, 0.022f, 0.039f, 0, color);
		}

		if (current)
			get_renderer()->draw_sprite(Texture("commonmenu", "shop_arrows_upanddown"), 0.937f + get_menu()->pos.x + (0.01f / 2) - 0.7f, ((position + 2) * 0.037f + 0.124f) + get_menu()->pos.y, 0.023f, 0.037f, 90.f, textColor);

		break;
	case SELECT:
		get_renderer()->draw_string(g_math.cprintf(Format, *number_op), get_menu()->scaled_x_coord + get_menu()->pos.x + 0.958f + (0.01f / 2) - 0.7f, ((position + 2) * 0.037f + 0.11f) + get_menu()->pos.y + 0.001f, get_renderer()->text_font, 0.37f, textColor, false, 2, current ? 0.727f : 0.711f);

		color = *bool_op ? RGBA(130, 214, 157, 255) : RGBA(200, 55, 80, 255);
		if (get_renderer()->toggle_style == 0) {
			if (*bool_op)
				get_renderer()->draw_sprite(Texture("commonmenu", get_menu()->is_selected(position) ? "shop_box_tickb" : "shop_box_tick"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.7f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001f, 0.022f, 0.039f, 0, { 255, 255, 255, 255 });
			else
				get_renderer()->draw_sprite(Texture("commonmenu", get_menu()->is_selected(position) ? "shop_box_blankb" : "shop_box_blank"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.7f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001f, 0.022f, 0.039f, 0, { 255, 255, 255, 255 });
		}
		else if (get_renderer()->toggle_style == 1) {
			get_renderer()->draw_sprite(Texture("commonmenu", "common_medal"), 0.952f + get_menu()->pos.x + (0.01f / 2) - 0.7f, (((position + 2) * 0.037f + 0.11f) + 0.01325f) + get_menu()->pos.y + 0.001f, 0.022f, 0.039f, 0, color);
		}

		if (current)
			get_renderer()->draw_sprite(Texture("commonmenu", "shop_arrows_upanddown"), 0.937f + get_menu()->pos.x + (0.01f / 2) - 0.7f, ((position + 2) * 0.037f + 0.124f) + get_menu()->pos.y, 0.023f, 0.037f, 90.f, textColor);

		break;
	case SCROLL:
	case SCROLLSELECT:
		get_renderer()->draw_string(g_math.cprintf(Format, *number_op), get_menu()->scaled_x_coord + get_menu()->pos.x + 0.958f + (0.01f / 2) - 0.7f, ((position + 2) * 0.037f + 0.11f) + get_menu()->pos.y + 0.001f, get_renderer()->text_font, 0.37f, textColor, false, 2, current ? 0.711f : 0.695f); // old: 0.015, 0.0

		if (current)
			get_renderer()->draw_sprite(Texture("commonmenu", "shop_arrows_upanddown"), 0.951f + get_menu()->pos.x + (0.01f / 2) - 0.7f, ((position + 2) * 0.037f + 0.124f) + get_menu()->pos.y, 0.023f, 0.037f, 90.f, textColor);

		break;
	}

	get_renderer()->draw_string(name.c_str(), 0.742f + get_menu()->pos.x - (0.01f / 2) - 0.7f + (get_menu()->is_selected(position) ? 0.0025f : 0.f), ((position + 2) * 0.037f + 0.11f) + get_menu()->pos.y + 0.001f, get_renderer()->text_font, 0.37f, textColor);

}



template <class Type>
void number_option<Type>::render_selected(int position) {
	current_op();
	static timer toggle_key(0ms);
	toggle_key.set_delay(150ms);
	if (get_menu()->enter_key_pressed && toggle_key.updated() && Requirement()) {
		switch (scroll_type) {
		case TOGGLE: *bool_op ^= true; break;
		case SELECT: *selected_number_op = *number_op; break;
		}

		function();
	}
	static timer left_key(0ms);
	left_key.set_delay(150ms);
	if (get_menu()->left_key_pressed && left_key.updated()) {
		*number_op -= step;
		if (loop_op) {
			if (*number_op < min) *number_op = max;
		}
		else if (*number_op < min && has_min) *number_op = min;
		if (scroll_type == SCROLL || (scroll_type == TOGGLE && *bool_op)) function();
	}
	static timer right_key(0ms);
	right_key.set_delay(150ms);
	if (get_menu()->right_key_pressed && right_key.updated()) {
		*number_op += step;
		if (loop_op) {
			if (*number_op > min) *number_op = max;
		}
		else if (*number_op > max && has_max) *number_op = max;
		if (scroll_type == SCROLL || (scroll_type == TOGGLE && *bool_op)) function();
	}



}

template class number_option<int>;
template class number_option<float>;
template class number_option<int64_t>;
template class number_option<int32_t>;
