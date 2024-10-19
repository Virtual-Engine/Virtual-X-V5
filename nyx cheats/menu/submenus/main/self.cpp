#include "self.hpp"
#include "../../submenus/home.hpp"

using namespace self_menu_vars;

namespace self_menu_vars {
	var vars;
	void godmod(bool toggle)
	{
		if (toggle)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 1);
			ENTITY::SET_ENTITY_PROOFS(PLAYER::PLAYER_PED_ID(), true, true, true, true, true, true, true, true);
		}
		else
		{
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 1);
		}
	}
	void invisible(bool toggle)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			if (toggle)
			{
				ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false, 0);
			}
			else ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true, 0);
		}

	}
	void spin_bot(bool toggle)
	{
		if (toggle)
		{
			static int Heading;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			ENTITY::SET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID(), Heading);
		}
		else
		{
			!NULL;
		}
	}
	void set_wanted_level(bool toggle, int level)
	{
		if (toggle) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), level, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
		}
	}
	void never_wanted(bool toggle)
	{
		if (toggle)
		{
			PLAYER::SET_MAX_WANTED_LEVEL(6);
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
			vars.m_level_number = 0;
		}
	}
	void set_fake_wanted_level(bool toggle, int level)
	{
		if (toggle)
		{
			MISC::SET_FAKE_WANTED_LEVEL(level);
		}
	}
}

void self_menu::initialize() {
	set_name("Self", true);
	set_parent_submenu<main_menu>();

	add_option(submenu_option("Animation")
		.add_submenu<main_menu>());

	add_option(submenu_option("Handtrails")
		.add_submenu<main_menu>());

	add_option(submenu_option("Models")
		.add_submenu<main_menu>());

	add_option(submenu_option("Movement")
		.add_submenu<main_menu>());

	add_option(submenu_option("Outfits")
		.add_submenu<main_menu>());

	add_option(submenu_option("Particles")
		.add_submenu<main_menu>());

	add_option(submenu_option("Vision")
		.add_submenu<main_menu>());

	add_option(submenu_option("Water")
		.add_submenu<main_menu>());

	add_option(break_option().get());

	add_option(toggle_option("Godmod")
		.add_toggle(vars.m_godmod));

	add_option(toggle_option("Invisible")
		.add_toggle(vars.m_invisible));

	add_option(toggle_option("Spin")
		.add_toggle(vars.m_spin_bot));

	add_option(number_option<int>(ScrollOptionType::SCROLL, "Opacity")
		.add_number(vars.m_opacity, "%d", 1).add_min(0).add_max(100).set_scroll_speed(20)
		.add_function([&] { ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 255 - (vars.m_opacity * 2.55f), false); })
		.add_tooltip("Set player opacity"));

	add_option(number_option<int>(ScrollOptionType::TOGGLE, "Level Wanted")
		.add_toggle(vars.m_level_wanted)
		.add_number(vars.m_level_number, "%d", 1).add_min(0).add_max(5)
		.add_tooltip("Define your level"));

	add_option(number_option<int>(ScrollOptionType::TOGGLE, "Fake Level Wanted")
		.add_toggle(vars.m_fake_wanted)
		.add_number(vars.m_fake_wanted_number, "%d", 1).add_min(0).add_max(6)
		.add_tooltip("Define your fake level"));

	add_option(toggle_option("Never Wanted")
		.add_toggle(vars.m_never_wanted));
}

void self_menu::update_once() {

}

void self_menu::update() {
}

void self_menu::feature_update() {
	godmod(vars.m_godmod);
	invisible(vars.m_invisible);
	spin_bot(vars.m_spin_bot);
	set_wanted_level(vars.m_level_wanted, vars.m_level_number);
	set_fake_wanted_level(vars.m_fake_wanted, vars.m_fake_wanted_number);
	never_wanted(vars.m_never_wanted);
}

self_menu* _instance;
self_menu* self_menu::get_instance() {
	if (_instance == nullptr) {
		_instance = new self_menu();
		_instance->initialize();
		get_submenu_handler()->add_submenu(_instance);
	}
	return _instance;
}
self_menu::~self_menu() { delete _instance; }