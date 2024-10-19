#pragma once
#include "../../common.hpp"
#include "../../invoker/natives.hpp"
#include "../../utilities/utilities.hpp"

#define VK_MOUSE_LEFT	0
#define VK_MOUSE_MIDDLE	2
#define VK_MOUSE_RIGHT	1

class mouse {
private:
	bool m_using_mouse;
	std::vector<std::function<void(int)>> m_scroll_events;
	struct {
		DWORD time;
		DWORD double_click_time;
		DWORD double_clicked;
		BOOL m_is_up_now;
		bool m_just_down;
	} button_states[5];

	const int m_NOWPERIOD = 100, m_MAXDOWN = 600000, m_DOUBLECLICKPERIOD = 200;

	void reset_button_state(DWORD button);
public:
	enum {
		NORMAL,
		INVIS = 2,
		HAND_OPEN,
		HAND_CLOSED,
		HAND_MIDDLEFINGER,
		ARROW_LEFT,
		ARROW_RIGHT,
		ARROW_UP,
		ARROW_DOWN,
		ARROW_LEFT_RIGHT,
		ARROW_PLUS,
		ARROW_MINUS
	};

	float m_scrollWheel;
	int m_X, m_Y, m_oldX, m_oldY;
	int m_resX, m_resY;

	void update_mouse();
	void scroll_wheel(int i);
	void enable_button(int key, bool is_up_now);
	bool button_down(DWORD button);
	bool button_just_up(DWORD button);
	bool button_just_down(DWORD button);
	bool button_double_clicked(DWORD button);
	float get_mouse_x();
	float get_mouse_y();
	float get_mouse_old_x();
	float get_mouse_old_y();
	float get_x_movement();
	float get_y_movement();
	Vector2 get_position_as_vector();

	bool mouse_within(float x, float y, float width, float height);
	bool mouse_within_centered(float x, float y, float width, float height);

	void set_cursor(int cursor) { HUD::SET_MOUSE_CURSOR_STYLE(cursor); }
	void reset_cursor() { HUD::SET_MOUSE_CURSOR_STYLE(0); }

	void register_scrollevent(std::function<void(int)> m_callback) {
		m_scroll_events.push_back(m_callback);
	}

	void toggle_mouse() { m_using_mouse ^= true; }
	bool can_use();
	bool is_enabled() { return m_using_mouse; }

};

mouse* get_mouse();