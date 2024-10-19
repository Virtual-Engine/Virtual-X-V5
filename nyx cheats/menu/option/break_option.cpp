#include "break_option.hpp"

void break_option::input(int position) {}

void break_option::render(int position) {
	HUD::SET_TEXT_CENTRE(1);
	get_renderer()->draw_string(line ? std::format("~p~__ ~s~{} ~p~__~s~", name).c_str() : name.c_str(), get_menu()->scaled_x_coord + get_menu()->pos.x, ((position + 2) * 0.037f + 0.11f) + get_menu()->pos.y + 0.001f, FontHouseScript, 0.37f, get_renderer()->sub_menu_title_color, false, -2);
}

void break_option::render_selected(int position) {
	switch (get_menu()->m_break_scroll) {
	case 1:
		if (get_menu()->m_scroll_offset > 0
			&& get_menu()->m_current_option - get_menu()->m_scroll_offset == 0)
			get_menu()->scaled_x_coord--;
		if (get_menu()->m_current_option - 1 < 0) {
			if (get_submenu_handler()->get_total_options() != 0) {
				get_menu()->m_current_option = get_menu()->m_max_options - 1;
				if (get_submenu_handler()->get_total_options() >= get_menu()->m_max_options)
					get_menu()->m_scroll_offset = get_submenu_handler()->get_total_options()
					- get_menu()->m_max_options;
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
				get_menu()->m_break_scroll = 3;
			}
		}
		else
			get_menu()->m_current_option--;
		break;
	case 2:
		if (get_menu()->m_scroll_offset
			< get_submenu_handler()->get_total_options() - get_menu()->m_max_options
			&& get_menu()->m_current_option + 1 - get_menu()->m_scroll_offset
			== get_menu()->m_max_options)
			get_menu()->m_scroll_offset++;
		if (get_menu()->m_current_option + 1 >= get_submenu_handler()->get_total_options()) {
			if (get_submenu_handler()->get_total_options() != 0) {
				get_menu()->m_current_option = 0;
				get_menu()->m_scroll_offset = 0;
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
				get_menu()->m_break_scroll = 4;
			}
		}
		else
			get_menu()->m_current_option++;
		break;
	case 3:
		if (get_menu()->m_scroll_offset > 0
			&& get_menu()->m_current_option - get_menu()->m_scroll_offset == 1)
			get_menu()->m_scroll_offset--;
		get_menu()->m_current_option--;
		break;
	case 4:
		if (get_menu()->m_scroll_offset
			< get_submenu_handler()->get_total_options() - get_menu()->m_max_options
			&& get_submenu_handler()->get_total_options() >= get_menu()->m_max_options
			&& get_menu()->m_current_option - get_menu()->m_scroll_offset
			== get_menu()->m_max_options)
			get_menu()->m_scroll_offset++;
		get_menu()->m_current_option++;
		break;
	}
}