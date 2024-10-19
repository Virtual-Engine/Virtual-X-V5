#include "../JSON/json.hpp"
#include "../invoker/Natives.hpp"
#include "../fiber/fiber.hpp"
#include "../fiber/queue.h"
#include "../menu/renderer.hpp"
#include "../menu/menu.hpp"

namespace VX
{
	inline Entity m_current_ent{};
	inline float m_heading{};
	inline Hash m_veh_model_hash{};
	inline bool m_running = false;
	inline bool m_shutdown_preview = false;
	inline std::chrono::time_point<std::chrono::steady_clock> m_rotation_start_time;

	Vector3 subtract(Vector3 a, Vector3 b) {
		Vector3 result;
		result.x = a.x - b.x;
		result.y = a.y - b.y;
		result.z = a.z - b.z;
		return result;
	}

	Vector3 multiply(Vector3 v, float scalar) {
		Vector3 result;
		result.x = v.x * scalar;
		result.y = v.y * scalar;
		result.z = v.z * scalar;
		return result;
	}

	Vector3 rotation_to_direction(Vector3 rotation) {
		float radPitch = rotation.x * (3.14159265358979323846 / 180.0f);
		float radYaw = rotation.z * (3.14159265358979323846 / 180.0f);

		Vector3 direction;
		direction.x = -sin(radYaw) * cos(radPitch);
		direction.y = cos(radYaw) * cos(radPitch);
		direction.z = sin(radPitch);

		return direction;
	}

	Vector3 add(Vector3 a, Vector3 b) {
		Vector3 result;
		result.x = a.x + b.x;
		result.y = a.y + b.y;
		result.z = a.z + b.z;
		return result;
	}

	bool request_model(Hash hash) {
		if (STREAMING::HAS_MODEL_LOADED(hash)) {
			return true;
		}

		bool has_loaded;

		if (STREAMING::IS_MODEL_VALID(hash) && STREAMING::IS_MODEL_IN_CDIMAGE(hash)) {
			do {
				has_loaded = STREAMING::HAS_MODEL_LOADED(hash);
				if (has_loaded)
					break;

				STREAMING::REQUEST_MODEL(hash);

				fiber::current()->sleep();
			} while (!has_loaded);

			return true;
		}

		return false;
	}

	Vehicle spawn(Hash hash, Vector3 location, float heading, bool is_networked, bool script_veh) {
		if (request_model(hash)) {
			auto veh =
				VEHICLE::CREATE_VEHICLE(hash, location.x, location.y, location.z, heading, is_networked, script_veh, false);

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

			return veh;
		}
		return 0;
	}

	inline void preview_vehicle(Hash hash) {
		if (m_running && m_veh_model_hash != hash) {
			if (m_running)
				m_shutdown_preview = true;
			return;
		}

		if (!m_running) {
			m_veh_model_hash = hash;
		}

		if (m_running) {
			return;
		}

		m_running = true;
		g_queue.add([&] {
			m_heading = 0.f;
			m_rotation_start_time = std::chrono::steady_clock::now();

			while (!m_shutdown_preview && g_running && get_menu()->is_opened
				&& !get_menu()->up_key_pressed && !get_menu()->down_key_pressed && !get_menu()->back_key_pressed) {
				Vector3 location{};

				if (m_current_ent == 0) {
					m_current_ent = spawn(m_veh_model_hash, location, 0.f, false, false);
					if (m_current_ent) {
						ENTITY::FREEZE_ENTITY_POSITION(m_current_ent, true);
						ENTITY::SET_ENTITY_ALPHA(m_current_ent, 170, false);
						ENTITY::SET_ENTITY_COLLISION(m_current_ent, false, false);
						ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(m_current_ent, false, false);
						ENTITY::SET_ENTITY_CAN_BE_DAMAGED(m_current_ent, false);
						ENTITY::SET_ENTITY_PROOFS(m_current_ent, true, true, true, true, true, true, true, true);
						ENTITY::SET_CAN_CLIMB_ON_ENTITY(m_current_ent, false);
						OBJECT::SET_OBJECT_ALLOW_LOW_LOD_BUOYANCY(m_current_ent, false);
					}
				}

				ENTITY::SET_ENTITY_HEADING(m_current_ent, m_heading);
				Vector3 minimum, maximum;
				MISC::GET_MODEL_DIMENSIONS(m_current_ent, &minimum, &maximum);
				Vector3 camera_rotation = CAM::GET_FINAL_RENDERED_CAM_ROT(0);
				Vector3 gameplay_camera_direction = rotation_to_direction(camera_rotation);
				Vector3 size = subtract(maximum, minimum); // Remplace 'maximum - minimum'
				Vector3 gameplay_camera_position = add(CAM::GET_FINAL_RENDERED_CAM_COORD(), multiply(gameplay_camera_direction, 5.f));
				Vector3 vehicle_spawn_position = add(gameplay_camera_position, multiply(gameplay_camera_direction, size.y));

				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(m_current_ent,
					vehicle_spawn_position.x,
					vehicle_spawn_position.y,
					vehicle_spawn_position.z,
					FALSE,
					FALSE,
					FALSE);

				get_renderer()->draw_in_world_box(m_current_ent, { 255, 255, 255, 255 });

				auto now = std::chrono::steady_clock::now();
				auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_rotation_start_time)
					.count()
					/ 1000.0;

				m_heading = (elapsed_time / 10.0) * 360.0;
				m_heading = fmod(m_heading, 360.0);

				fiber::current()->sleep();
			}

			ENTITY::DELETE_ENTITY(&m_current_ent);

			m_veh_model_hash = {};
			m_shutdown_preview = false;
			m_running = false;
			m_current_ent = NULL;
			});
	}
}