#pragma once
#include "../../common.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/submenu_handler.hpp"
#include "../../invoker/natives.hpp"
#include "../../menu/util/keyboard.hpp"
#include "../../gta/enums.hpp"

static bool IsOpenPressed() {
	return (KeyJustUp(get_menu()->m_open_key));
}
static bool IsAJPressed(bool overrideInput = false) {
	if (!overrideInput) return false;
	return PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, (int)ControllerInputs::INPUT_FRONTEND_ACCEPT) || get_keyboard()->key_just_up(VK_NUMPAD5);
}

static bool IsBJPressed(bool overrideInput = false) {
	if (!overrideInput) return false;
	return PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, (int)ControllerInputs::INPUT_FRONTEND_CANCEL) || KeyJustDown(VK_NUMPAD0);
}

static bool IsRPressed(bool overrideInput = false) {
	if (!overrideInput) return false;
	if (PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_RIGHT) || KeyDown(VK_NUMPAD6)) return true; else return false;
}

static bool IsRJPressed(bool overrideInput = false) {
	if (!overrideInput) return false;
	if (PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, (int)ControllerInputs::INPUT_FRONTEND_RIGHT) || KeyJustUp(VK_NUMPAD6)) return true; else return false;
}

static bool IsLPressed(bool overrideInput = false) {
	if (!overrideInput) return false;
	if (PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_LEFT) || KeyDown(VK_NUMPAD4)) return true; else return false;
}

static bool IsUPressed(bool overrideInput = false) {
	if (!overrideInput) return false;
	if (PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_UP) || KeyDown(VK_NUMPAD8)) return true; else return false;
}

static bool IsDPressed(bool overrideInput = false) {
	if (!overrideInput) return false;
	if (PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_FRONTEND_DOWN) || KeyDown(VK_NUMPAD2)) return true; else return false;
}
static bool IsOptionPressed() {
	if (IsAJPressed()) {
		AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
		return true;
	}
	return false;
}