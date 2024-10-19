#pragma once
#include "../common.hpp"
#include "../invoker/natives.hpp"
#include "../utilities/input.hpp"
#include "../gta/enums.hpp"
#include "../utilities/timer.h"
#include "../menu/submenu_handler.hpp"
#include "../menu/submenu.hpp"
#include "../menu/renderer.hpp"
#include "../fiber/queue.h"

class menu
{
public:
	void renderer();
	static void update();

public:
	void disable_controls();
	void reset_key();
	void check_key();
	void handle_key();

	bool is_open() const {
		return this->is_opened;
	}
	void toggle(bool state) {
		this->is_opened = state;
	}

	bool is_selected(int sub) {
		return current_rendering_scroll == (sub + 1);
	}

	int m_max_options = 14;
	int m_current_option = 0;
	int previous_option = 0;
	int current_rendering_scroll;
	int m_scroll_offset;
	int m_break_scroll;

public:
	bool open_key_pressed = false;
	bool back_key_pressed = false;
	bool enter_key_pressed = false;
	bool up_key_pressed = false;
	bool down_key_pressed = false;
	bool left_key_pressed = false;
	bool right_key_pressed = false;
	bool is_opened = false;
	bool unloading_key_press = false;

	bool left_press;
	bool right_press;

	bool m_draggingSlider = false;

	int m_open_key = VK_F4;
	int m_mouse_key = 'X';

	bool m_isFading;
	bool m_closing = false;

	float m_opacity = 0;

	inline static float m_deltaTime;
	inline static DWORD m_lastFrameTime;

	void fadeIn(std::function<void(void)> callback) {
		g_queue.add([this, callback] {
			while (m_opacity < 1.0f) {
				m_isFading = true;
				m_opacity = g_math.Clamp(m_opacity + 1.5f * m_deltaTime, 0.0, 1.0); fiber::current()->sleep();
			}callback(); m_isFading = false; });
	}

	void fadeOut(std::function<void(void)> callback) {
		g_queue.add([this, callback] {
			while (m_opacity > 0.0f) {
				m_isFading = true;
				m_opacity = g_math.Clamp(m_opacity - 1.5f * m_deltaTime, 0.0, 1.0); fiber::current()->sleep();
			}callback(); m_isFading = false; });
	}

	void open() {
		if (m_isFading) return;
		is_opened = true;
		//AUDIO::PLAY_SOUND_FRONTEND(-1, "FocusIn", "HintCamSounds", 0);
		//fadeIn([&] {m_closing = false; });
		m_closing = false;
	}
	void close() {
		if (!m_isFading) { m_closing = true; //fadeOut([&] { is_opened = false; }); 
			is_opened = false;
		};
	}
	void toggle_open() { if (!m_isFading) { if (is_opened) close(); else open(); } }

	Vector2 pos;
	Vector2 m_size;
	Vector2 m_position;
	inline static float scaled_x_coord;
};

menu* get_menu();