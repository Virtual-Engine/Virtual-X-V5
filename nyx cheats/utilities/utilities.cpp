#define _CRT_SECURE_NO_WARNINGS
#include "utilities.hpp"
#include <bitset>
#include <shlobj.h>

#include "../invoker/natives.hpp"
#include "../fiber/queue.h"
#include "../fiber/fiber.hpp"

#pragma comment(lib, "shell32.lib")
namespace VX {

	std::string utility::get_documents_path() {
		wchar_t Folder[1024];
		HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, Folder);
		if (SUCCEEDED(hr))
		{
			char str[1024];
			wcstombs(str, Folder, 1023);
			return str;
		}
		else return "";
	}
	void utility::DoTimedFunction(int* timer, int timeMS) {
		if (*timer < MISC::GET_GAME_TIMER()) {
			*timer = MISC::GET_GAME_TIMER() + timeMS;
		}
	}
	void utility::SimpleRequestModel(int model) {
		int tries = 0;
		while (!STREAMING::HAS_MODEL_LOADED(model) && tries < 25) {
			STREAMING::REQUEST_MODEL(model);
			tries++;
			fiber::current()->sleep();
		}
	}
	Hash utility::GetHashKey(String str) {
		const auto len = strlen(str);
		unsigned int hash, i;
		for (hash = i = 0; i < len; ++i) {
			hash += tolower(str[i]);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}
	RGBA utility::HSVToRGB(float h, float s, float v) {
		float r = 0, g = 0, b = 0;

		if (s == 0) {
			r = v;
			g = v;
			b = v;
		}
		else {
			int i;
			double f, p, q, t;

			if (h == 360)h = 0;
			else h = h / 60;

			i = (int)trunc(h);
			f = h - i;

			p = v * (1.0f - s);
			q = v * (1.0f - (s * f));
			t = v * (1.0f - (s * (1.0f - f)));

			switch (i) {
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			default: r = v; g = p; b = q; break;
			}

		}
		return { (int)(r * 255), (int)(g * 255), (int)(b * 255), 255 };
	}
	void utility::load_ytd(const char* file_name) {
		int index = 0;
		char buffer[256];
		sprintf(buffer, "%s\\Virtual X\\textures\\%s", g_utility.get_documents_path(), file_name);
		//GetCore()->m_register_file(& index, buffer, true, file_name, false);
		ZeroMemory(buffer, sizeof(buffer));
	}
	void utility::notifyMap(const char* fmt, ...)
	{
		char buf[2048] = { 0 };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(buf, fmt, va_alist);
		va_end(va_alist);

		char buff2[2048] = { 0 };
		sprintf_s(buff2, "%s", buf);

		HUD::SET_TEXT_OUTLINE();
		HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buff2);
		HUD::END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_WITH_CREW_TAG_AND_ADDITIONAL_ICON("VX", (char*)"notify", true, 5, (char*)"~p~Virtual Engine Service", (char*)"", 1.f, (char*)"", 5, 0);
	}	void notifyMap(std::string str) { notifyMap(&str[0]); }

	void utility::Animation(const char* anim, const char* animid)
	{
		int pPlayer = PLAYER::PLAYER_PED_ID();
		STREAMING::REQUEST_ANIM_DICT(anim);
		if (STREAMING::HAS_ANIM_DICT_LOADED((anim)))
		{
			TASK::TASK_PLAY_ANIM(pPlayer, anim, animid, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
		}
	}
	
	void utility::ChangeModel(const char* To_Change)
	{
		Hash Model_To_Change = MISC::GET_HASH_KEY(To_Change);
		STREAMING::REQUEST_MODEL(Model_To_Change);
		{
			Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			Coordinates.x + 2;
			if (STREAMING::IS_MODEL_IN_CDIMAGE(Model_To_Change))
			{
				fiber::current()->sleep();
					if (STREAMING::IS_MODEL_VALID(Model_To_Change))
					{
						Ped PED_Spawn = PED::CREATE_PED(26, Model_To_Change, Coordinates.x, Coordinates.y, Coordinates.z, 0, true, true);
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED_Spawn);
						auto NET_ID = NETWORK::PED_TO_NET(PED_Spawn);
						DECORATOR::DECOR_SET_INT(PED_Spawn, (char*)"MPBitset", 0);
						NETWORK::NETWORK_REGISTER_ENTITY_AS_NETWORKED(PED_Spawn);
						ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(PED_Spawn, false);
						if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(PED_Spawn))
							NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NET_ID, true);
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(PED_Spawn, true, true);
						NETWORK::SET_NETWORK_ID_CAN_MIGRATE(NETWORK::PED_TO_NET(PED_Spawn), TRUE);
						if (!DECORATOR::DECOR_EXIST_ON(PED_Spawn, "MissionType"))
						{
							DECORATOR::DECOR_REGISTER("MissionType", 3);
							DECORATOR::DECOR_SET_INT(PED_Spawn, "MissionType", 0);

						}
						Ped PED_TO_DELETE = PLAYER::PLAYER_PED_ID();
						PLAYER::CHANGE_PLAYER_PED(PLAYER::PLAYER_ID(), PED_Spawn, true, true);
						ENTITY::DELETE_ENTITY(&PED_TO_DELETE);
					}
					
			}
		}
	}
	inline void RequestControlOfid(Entity netid)
	{
		int tick = 0;

		while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
			tick++;
		}
	}
	void utility::RequestControlOfEnt(Entity entity)
	{
		int tick = 0;
		while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
			tick++;
		}
		if (NETWORK::NETWORK_IS_SESSION_STARTED())
		{
			int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
			RequestControlOfid(netID);
			NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
		}
	}
	bool utility::is_player_in_interior(Player player)
	{
		auto player_ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
		bool interior = INTERIOR::GET_INTERIOR_FROM_ENTITY(player_ped);

		return INTERIOR::IS_VALID_INTERIOR(interior);
	}

	bool utility::is_player_friend(Player player)
	{
		int handle[13];
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);

		if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
			return NETWORK::NETWORK_IS_FRIEND(&handle[0]);

		return NULL;
	}
	void utility::SetVehicleWheelColor(int color) {
		int colTemp, nullCol;
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
		VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &colTemp, &nullCol);
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, colTemp, color);
	}
	Vehicle utility::vehicle(bool include_last) {
		auto vehicle{ PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), include_last)};
		if (include_last) {
			return vehicle;
		}
		else {
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
				return vehicle;
			}
			else {
				return -1;
			}
		}
	}
	Vector3 utility::coords(Entity entity) {
		return ENTITY::GET_ENTITY_COORDS(entity == 0 ? PLAYER::PLAYER_PED_ID() : entity, 0);
	}

	bool utility::has_string_attached(const std::string& first, const std::string& check) {
		std::string lower{ first };
		std::string check_lower{ check };
		std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
			return std::tolower(c);
			});
		std::transform(check_lower.begin(), check_lower.end(), check_lower.begin(), [](unsigned char c) {
			return std::tolower(c);
			});

		size_t found{ lower.find(check_lower) };
		if (found != std::string::npos) {
			return true;
		}
		else {
			return false;
		}
	}
	bool utility::is_ped_shooting(Ped ped) {
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, 1);
		return PED::IS_PED_SHOOTING_IN_AREA(ped, coords.x, coords.y, coords.z, coords.x, coords.y, coords.z, true, true);
	}
	
	const char* utility::fixed_decimel(float number) {
		double intpart;
		double fracpart = std::modf(number, &intpart);
		std::stringstream ss;
		ss << intpart;
		std::string int_part = ss.str();
		std::stringstream s;
		s << fracpart;
		std::string frac_part = s.str();
		frac_part.erase(0, 1);
		std::string text = int_part + frac_part;
		return text.c_str();
	}
	void utility::DoTimedFunctionV2(int* timer, int timeMS) {
		if (*timer < MISC::GET_GAME_TIMER()) {
			*timer = MISC::GET_GAME_TIMER() + timeMS;
		}
	}
	constexpr char ToLower(const char c)
	{
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}
	std::uint32_t utility::Joaat(const char* str)
	{
		std::uint32_t hash = 0;
		while (*str)
		{
			hash += ToLower(*str++);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
	const char* gta_utility::draw_keyboard() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(1, "", "", "", "", "", "", 10);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			fiber::current()->sleep();
		}
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) {
			return 0;
		}
		return MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	

	const char* gta_utility::get_vehicle_class_name(int id) {
		std::stringstream ss; ss << "VEH_CLASS_" << id;
		return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(ss.str().c_str()) == "NULL" ? "Unknown Class" : HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(ss.str().c_str());
	}

	void gta_utility::request_model(std::uint32_t hash) {
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash)) {
			fiber::current()->sleep();
		}
	}

	Vehicle gta_utility::spawn_vehicle(std::uint32_t hash) {
		if (!STREAMING::IS_MODEL_VALID(hash)) {
			return NULL;
		}
		request_model(hash);
		float forward = 5.f;
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		float x = forward * sin(g_math.deg_to_rad(heading)) * -1.f;
		float y = forward * cos(g_math.deg_to_rad(heading));
		//*(unsigned short*)get_pointers()->m_set_this_thread_networked = 0x9090; // we set this thread as networked so we can spawn the vehicle / u can add rage classes and use excute under thread instead of this
		Vehicle the_vehicle = VEHICLE::CREATE_VEHICLE(hash, coords.x + x, coords.y + y, coords.z, heading, NETWORK::NETWORK_IS_SESSION_ACTIVE(), false, false);
		std::int32_t net_id = NETWORK::VEH_TO_NET(the_vehicle);
		//*(unsigned short*)get_pointers()->m_set_this_thread_networked = 0x0574; // We restore it so we don't get detected 
		NETWORK::NETWORK_FADE_IN_ENTITY(the_vehicle, true, 0); 
		NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(net_id, TRUE);
		if (ENTITY::IS_ENTITY_VISIBLE_TO_SCRIPT(NETWORK::NET_TO_VEH(net_id))) {
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), NETWORK::NET_TO_VEH(net_id), -1);
			float speed = ENTITY::GET_ENTITY_SPEED(the_vehicle);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(NETWORK::NET_TO_VEH(net_id), speed);
			VEHICLE::SET_VEHICLE_ENGINE_ON(NETWORK::NET_TO_VEH(net_id), TRUE, TRUE, TRUE);
			DECORATOR::DECOR_SET_INT(NETWORK::NET_TO_VEH(net_id), "MPBitset", (1 << 10));
			return the_vehicle;
		}
		if (Sitinvehicle) {
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), the_vehicle, -1);
		}
		if (g_SpawnInvinc) {
			ENTITY::SET_ENTITY_INVINCIBLE(the_vehicle, true);
		}
		if (MaxVehiclespawn) {
			VEHICLE::SET_VEHICLE_MOD_KIT(the_vehicle, 0);
			VEHICLE::TOGGLE_VEHICLE_MOD(the_vehicle, 18 /* Turbo */, TRUE);
			VEHICLE::TOGGLE_VEHICLE_MOD(the_vehicle, 20 /* Tire Smoke */, TRUE);
			VEHICLE::TOGGLE_VEHICLE_MOD(the_vehicle, 17 /* Xenon Headlights */, TRUE);
			VEHICLE::SET_VEHICLE_WINDOW_TINT(the_vehicle, 1);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(the_vehicle, false);
			for (int i = 0; i < 50; i++) {
				VEHICLE::SET_VEHICLE_MOD(the_vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(the_vehicle, i) - 1, true);
			}
		}
		return NULL;
	}

	float math::deg_to_rad(float degs) {
		return degs * 3.141592653589793f / 180.f;
	}

	float math::convert_360(float base, float min, float max) {
		float fVar0;
		if (min == max) return min;
		fVar0 = max - min;
		base -= std::round(base - min / fVar0) * fVar0;
		if (base < min) base += fVar0;
		return base;
	}
}
