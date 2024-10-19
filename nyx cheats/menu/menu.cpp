#include "menu.hpp"
#include "util/mouse.hpp"

menu g_menu;

void menu::handle_key() {
	static timer unload_key(0ms);
	unload_key.set_delay(150ms);
	if (this->unloading_key_press && unload_key.updated()) {
		g_running = false;
	}

	static timer up_key(0ms);
	up_key.set_delay(150ms);
	if (this->is_open() && this->up_key_pressed && up_key.updated()) {
		if (this->m_current_option == 0) {
			if (get_submenu_handler()->get_total_options() != 0) {
				this->m_current_option = get_submenu_handler()->get_total_options() - 1;
				if (get_submenu_handler()->get_total_options() >= this->m_max_options)
					this->m_scroll_offset = get_submenu_handler()->get_total_options() - this->m_max_options;
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
				this->m_break_scroll = 3;
			}
		}
		else {
			if (get_submenu_handler()->get_total_options() != 0) {
				this->m_current_option--;
				if (this->m_scroll_offset > 0 && this->m_current_option - this->m_scroll_offset == -1)
					this->m_scroll_offset--;
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
				this->m_break_scroll = 1;
			}
		}
	}

	static timer down_key(0ms);
	down_key.set_delay(150ms);
	if (this->is_open() && this->down_key_pressed && down_key.updated()) {
		if (this->m_current_option == get_submenu_handler()->get_total_options() - 1) {
			if (get_submenu_handler()->get_total_options() != 0) {
				this->m_current_option = 0;
				this->m_scroll_offset = 0;
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
				this->m_break_scroll = 4;
			}
		}
		else {
			if (get_submenu_handler()->get_total_options() != 0) {
				this->m_current_option++;
				if (this->m_scroll_offset < get_submenu_handler()->get_total_options() - this->m_max_options && this->m_current_option - this->m_scroll_offset == this->m_max_options)
					this->m_scroll_offset++;
				if (get_submenu_handler()->get_total_options() >= this->m_max_options
					&& this->m_scroll_offset > get_submenu_handler()->get_total_options() - this->m_max_options)
					this->m_scroll_offset = get_submenu_handler()->get_total_options() - this->m_max_options;
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
				this->m_break_scroll = 2;
			}
		}
	}

	static timer back_key(0ms);
	back_key.set_delay(150ms);
	if (this->is_open() && this->back_key_pressed && back_key.updated()) {
		get_submenu_handler()->set_sub_previous();
	}
}

void menu::check_key()
{
	reset_key();
	this->open_key_pressed = (PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_RB) && PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_RIGHT)) || get_input()->is_key_pressed(VK_F5);
	this->down_key_pressed = PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_DOWN) || get_input()->is_key_pressed(VK_NUMPAD2);
	this->up_key_pressed = PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_UP) || get_input()->is_key_pressed(VK_NUMPAD8);
	this->left_key_pressed = PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_LEFT) || get_input()->is_key_pressed(VK_NUMPAD4);
	this->right_key_pressed = PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_RIGHT) || get_input()->is_key_pressed(VK_NUMPAD6);
	this->enter_key_pressed = PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_ACCEPT) || get_input()->is_key_pressed(VK_NUMPAD5);
	this->back_key_pressed = PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_CANCEL) || get_input()->is_key_pressed(VK_NUMPAD0);
	this->unloading_key_press = get_input()->is_key_pressed(VK_END);
}

void menu::reset_key() {
	this->open_key_pressed = false;
	this->back_key_pressed = false;
	this->enter_key_pressed = false;
	this->up_key_pressed = false;
	this->down_key_pressed = false;
	this->left_key_pressed = false;
	this->right_key_pressed = false;
}

void menu::disable_controls() {
	HUD::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(1);

	PAD::SET_INPUT_EXCLUSIVE(2, (int)ControllerInputs::INPUT_FRONTEND_ACCEPT);
	PAD::SET_INPUT_EXCLUSIVE(2, (int)ControllerInputs::INPUT_FRONTEND_CANCEL);
	PAD::SET_INPUT_EXCLUSIVE(2, (int)ControllerInputs::INPUT_FRONTEND_DOWN);
	PAD::SET_INPUT_EXCLUSIVE(2, (int)ControllerInputs::INPUT_FRONTEND_UP);
	PAD::SET_INPUT_EXCLUSIVE(2, (int)ControllerInputs::INPUT_FRONTEND_LEFT);
	PAD::SET_INPUT_EXCLUSIVE(2, (int)ControllerInputs::INPUT_FRONTEND_RIGHT);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_NEXT_CAMERA, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_VEH_SELECT_NEXT_WEAPON, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_FRONTEND_RIGHT, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_FRONTEND_DOWN, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_FRONTEND_LEFT, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_FRONTEND_UP, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_ARREST, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_CONTEXT, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_MELEE_BLOCK, true);
	PAD::DISABLE_CONTROL_ACTION(2, (int)ControllerInputs::INPUT_FRONTEND_UP, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_HUD_SPECIAL, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_VEH_CIN_CAM, true);
	PAD::DISABLE_CONTROL_ACTION(2, (int)ControllerInputs::INPUT_VEH_CIN_CAM, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_VEH_HEADLIGHT, true);
	PAD::DISABLE_CONTROL_ACTION(2, (int)ControllerInputs::INPUT_FRONTEND_LEFT, true);
	PAD::DISABLE_CONTROL_ACTION(2, (int)ControllerInputs::INPUT_FRONTEND_DOWN, true);
	PAD::DISABLE_CONTROL_ACTION(2, (int)ControllerInputs::INPUT_FRONTEND_RDOWN, true);
	PAD::DISABLE_CONTROL_ACTION(2, (int)ControllerInputs::INPUT_FRONTEND_RIGHT, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_CHARACTER_WHEEL, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_VEH_RADIO_WHEEL, true);
	PAD::DISABLE_CONTROL_ACTION(2, (int)ControllerInputs::INPUT_FRONTEND_CANCEL, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_MELEE_ATTACK_LIGHT, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_MELEE_ATTACK_HEAVY, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_SELECT_CHARACTER_TREVOR, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_SELECT_CHARACTER_MICHAEL, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_SELECT_CHARACTER_FRANKLIN, true);
	PAD::DISABLE_CONTROL_ACTION(0, (int)ControllerInputs::INPUT_SELECT_CHARACTER_MULTIPLAYER, true);
}

void menu::renderer()
{
	float sliderX = m_position.x - m_size.x / 2 - 0.01f;
	float background_height = (0.040f + ((0.037f + 0.00010f) * (get_renderer()->render_options - 1)));
	float sliderTop = get_menu()->pos.y - background_height / 2;
	float sliderPosition = (((float)(get_submenu_handler()->current_option_without_breaks) / ((float)get_submenu_handler()->total_options_without_breaks - 1)) * (background_height - get_renderer()->option_height)) + get_renderer()->option_height / 2;
	if (get_mouse()->mouse_within_centered(sliderX, sliderTop + sliderPosition, 0.008f, 0.022f)) {
		get_mouse()->set_cursor(mouse::HAND_OPEN);
		if (get_mouse()->button_down(VK_MOUSE_LEFT)) {
			m_draggingSlider = true;
		}
	}
	if (m_draggingSlider) {
		get_mouse()->set_cursor(mouse::HAND_CLOSED);
		float posback = (get_mouse()->get_mouse_y() + 0.022f / 2 - sliderTop) / (background_height - get_renderer()->option_height);
		float positionback = posback * get_submenu_handler()->total_options_without_breaks;
		g_math.Clamp(&positionback, 0, get_submenu_handler()->get_total_options() - 1);
		m_current_option = positionback;
		if (m_scroll_offset > m_current_option) m_scroll_offset -= m_scroll_offset - m_current_option;
		else if (m_current_option > m_scroll_offset + m_max_options - 1) m_scroll_offset += m_current_option - (m_scroll_offset + m_max_options - 1);
	}
}

void menu::update()
{
	while (true) {
		fiber::current()->sleep();
	}
}

menu* get_menu() { return &g_menu; }