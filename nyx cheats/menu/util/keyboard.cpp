#include "keyboard.hpp"
#include "../../invoker/natives.hpp"

static keyboard g_keyboard;

void keyboard::on_keyboard_message(DWORD key, WORD repeats, uint8_t scan_code, BOOL is_extended, BOOL is_with_alt, BOOL was_down_before, BOOL is_up_now) {
	/*Numpad shift fix*/
	if (key == 38) key = 104;
	if (key == 39) key = 102;
	if (key == 37) key = 100;
	if (key == 40) key = 98;
	if ((int)key < KEYSIZE) {
		get_keyboard()->m_keyStates[key].time = GetTickCount();
		get_keyboard()->m_keyStates[key].m_isWithAlt = is_with_alt;
		get_keyboard()->m_keyStates[key].m_wasDownBefore = was_down_before;
		get_keyboard()->m_keyStates[key].m_isUpNow = is_up_now;
		if (!get_keyboard()->m_keyStates[key].m_wasDownBefore) get_keyboard()->m_keyStates[key].m_justDown = !is_up_now;
		if (is_up_now) get_keyboard()->m_lastKey = key;
	}
}

void keyboard::reset_key_state(DWORD key) {
	if ((int)key < KEYSIZE) memset(&m_keyStates[key], 0, sizeof(m_keyStates[0]));
}

bool keyboard::key_down(DWORD key) {
	if (!HUD::IS_MP_TEXT_CHAT_TYPING() && !HUD::IS_SOCIAL_CLUB_ACTIVE()) {
		return ((int)key < KEYSIZE) ? ((GetTickCount() < m_keyStates[key].time + m_MAXDOWN) && !m_keyStates[key].m_isUpNow) : false;
	}
	return false;
}

bool keyboard::key_just_up(DWORD key, bool exclusive) {
	if (!HUD::IS_MP_TEXT_CHAT_TYPING() && !HUD::IS_SOCIAL_CLUB_ACTIVE()) {
		bool result = ((int)key < KEYSIZE) ? (GetTickCount() < m_keyStates[key].time + m_NOWPERIOD && m_keyStates[key].m_isUpNow) : false;
		if (result && exclusive) reset_key_state(key);
		return result;
	}
	return false;
}

bool keyboard::key_just_down(DWORD key) {
	bool result = GetTickCount() - m_NOWPERIOD < m_keyStates[key].time && !m_keyStates[key].m_isUpNow && m_keyStates[key].m_justDown;
	if (result) m_keyStates[key].m_justDown = false;
	return result;
}

keyboard* get_keyboard() {
	return &g_keyboard;
}

bool KeyDown(DWORD key) {
	return get_keyboard()->key_down(key);
}
bool KeyJustUp(DWORD key, bool exclusive) {
	return get_keyboard()->key_just_up(key, exclusive);
}

bool KeyJustDown(DWORD key) {
	return get_keyboard()->key_just_down(key);
}