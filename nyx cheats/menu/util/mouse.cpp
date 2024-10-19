#include "mouse.hpp"

static mouse g_mouse;

void mouse::update_mouse() {
	m_oldX = m_X;
	m_oldY = m_Y;
	GRAPHICS::GET_ACTUAL_SCREEN_RESOLUTION(&m_resX, &m_resY);
}

/*Do Mouse scrolling shit here*/
void mouse::scroll_wheel(int i) {
	if (m_using_mouse) {
		for (auto& event : m_scroll_events) {
			event(i);
		}
	}
}

void mouse::enable_button(int button, bool down) {
	button_states[button].double_clicked = (button < 5 && button_states[button].m_is_up_now && GetTickCount() < button_states[button].time + m_DOUBLECLICKPERIOD);
	button_states[button].time = GetTickCount();
	button_states[button].m_is_up_now = !down;
	button_states[button].m_just_down = down;
}

bool mouse::button_down(DWORD button) {
	if (!m_using_mouse) return false;
	return (button < 5) ? ((GetTickCount() < button_states[button].time + m_MAXDOWN) && !button_states[button].m_is_up_now) : false;
}

void mouse::reset_button_state(DWORD button) {
	if (button < 5) memset(&button_states[button], 0, sizeof(button_states[0]));
}

bool mouse::button_just_up(DWORD button) {
	if (!m_using_mouse) return false;
	const auto result = (button < 5) ? (GetTickCount() < button_states[button].time + m_NOWPERIOD && button_states[button].m_is_up_now) : false;
	if (result) reset_button_state(button);
	return result;
}

bool mouse::button_just_down(DWORD button) {
	if (!m_using_mouse) return false;
	bool result = (button < 5) ? (GetTickCount() - m_NOWPERIOD < button_states[button].time && !button_states[button].m_is_up_now && button_states[button].m_just_down) : false;
	if (result) button_states[button].m_just_down = false;
	return result;
}

bool mouse::button_double_clicked(DWORD button) {
	if (!m_using_mouse) return false;
	bool result = (button < 5) ? ((GetTickCount() < button_states[button].time + m_NOWPERIOD) && button_states[button].double_clicked) : false;
	if (result) reset_button_state(button);
	return result;
}

float mouse::get_mouse_x() { return m_using_mouse ? (float)m_X / m_resX : -1; }
float mouse::get_mouse_y() { return m_using_mouse ? (float)m_Y / m_resY : -1; }
float mouse::get_mouse_old_x() { return m_using_mouse ? (float)m_oldX / m_resX : -1; }
float mouse::get_mouse_old_y() { return m_using_mouse ? (float)m_oldY / m_resY : -1; }
float mouse::get_x_movement() { return m_using_mouse ? get_mouse_x() - get_mouse_old_x() : -1; }
float mouse::get_y_movement() { return m_using_mouse ? get_mouse_y() - get_mouse_old_y() : -1; }
Vector2 mouse::get_position_as_vector() { return { get_mouse_x(),get_mouse_y() }; }

bool mouse::mouse_within(float x, float y, float width, float height) {
	if (!m_using_mouse) return false;
	return g_math.within(get_mouse_x(), x, x + width) && g_math.within(get_mouse_y(), y, y + height);
}

bool mouse::mouse_within_centered(float x, float y, float width, float height) {
	if (!m_using_mouse) return false;
	return g_math.within(get_mouse_x(), x - width / 2, x + width / 2) && g_math.within(get_mouse_y(), y - height / 2, y + height / 2);
}

bool mouse::can_use() { return m_using_mouse; }

mouse* get_mouse() { return &g_mouse; }