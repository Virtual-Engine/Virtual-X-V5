#pragma once
#include "../common.hpp"
#include "../rage/classes.hpp"
#include "../invoker/natives.hpp"
#include <functional>
namespace VX {

	typedef const char* String;

	class utility
	{
	 public:
		std::string get_documents_path();
		void notifyMap(const char* fmt, ...);
		void Animation(const char* anim, const char* animid);
		void ChangeModel(const char* To_Change);
		void RequestControlOfEnt(Entity entity);
		bool is_player_in_interior(Player player);
		bool is_player_friend(Player player);
		void SetVehicleWheelColor(int color);
		Vehicle vehicle(bool includeLast = false);
		Vector3 coords(Entity entity = 0);
		bool has_string_attached(const std::string& first, const std::string& check);
		bool is_ped_shooting(Ped ped);
		std::uint32_t Joaat(const char* str);
		void load_ytd(const char* file_name);
		const char* fixed_decimel(float number);
		void SetHeader(int header);
		RGBA HSVToRGB(float h, float s, float v);
		void DoTimedFunction(int* timer, int timeMS);
		void DoTimedFunctionV2(int* timer, int timeMS);
		Hash GetHashKey(String str);
		void SimpleRequestModel(int model);
	
	};
	inline utility g_utility;

	class math {
	public:
		float convert_360(float base, float min, float max);
		float deg_to_rad(float degs);
		int Clamp(int val, int min, int max) {
			return val < min ? min : val > max ? max : val;
		}
		float Clamp(float val, float min, float max) {
			return val < min ? min : val > max ? max : val;
		}
		void Clamp(int* val, int min, int max) {
			*val = *val < min ? min : *val > max ? max : *val;
		}
		bool Clamp(float* val, float min, float max) {
			*val = *val < min ? min : *val > max ? max : *val;
			return *val == min || *val == max;
		}
		template<typename T>
		bool within(T val, T min, T max) {
			return val <= max && val >= min;
		}
		void do_timed_function(int* timer, int timeMS, std::function<void()> function) {
			if (*timer < MISC::GET_GAME_TIMER()) {
				*timer = MISC::GET_GAME_TIMER() + timeMS;
				function();
			}
		}
		std::string toLower(std::string data) {
			std::transform(data.begin(), data.end(), data.begin(), ::tolower);
			return data;
		}
		char* cprintf(const char* format, ...) {
			std::memset(vaBuffer, 0, 0x1000);
			va_list ap;
			va_start(ap, format);
			vsprintf_s(vaBuffer, format, ap);
			va_end(ap);
			return vaBuffer;
		}
	};
	inline math g_math;

	class gta_utility {
	public:
		bool MaxVehiclespawn = true;
		bool Sitinvehicle = true;
		bool g_SpawnInvinc = false;
		const char* draw_keyboard();
		const char* get_vehicle_class_name(int id);
		void request_model(std::uint32_t hash);
		Vehicle spawn_vehicle(std::uint32_t hash);
		rage::scrProgram* get_program_by_hash(std::uint32_t hash);
	};
	inline gta_utility g_gta_utility;
}