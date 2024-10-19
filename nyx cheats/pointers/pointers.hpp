#pragma once
#include "../common.hpp"
#include "../rage/classes.hpp"
#include "../memory/all.hpp"

#include <dxgi.h>

namespace rage {
	class CDynamicEntity;
	enum class eLoadingScreenState : int32_t
	{
		Invalid = -1,
		Finished,
		PreLegal,
		Unknown_2,
		Legals,
		Unknown_4,
		LandingPage,
		Transition,
		Unknown_7,
		Unknown_8,
		Unknown_9,
		SessionStartLeave
	};
}
namespace VX {
	namespace types {
		using script_virtual_machine = std::int64_t(void* stack, std::int64_t** globals, std::int64_t* program, std::int64_t* thread_context);
		using get_native_handler = rage::scrNativeHandler(rage::scrNativeRegistrationTable*, std::uint64_t);
		using fix_vectors = void (*)(rage::scrNativeCallContext* call_ctx);
		using ptr_to_handle = Entity(*)(void*);
		using handle_to_ptr = rage::CDynamicEntity* (*)(Entity);
	};
	class pointers
	{
	public:
		void initialize();
	public:
		rage::eLoadingScreenState* m_loading_screen_state{};
		types::script_virtual_machine* m_script_virtual_machine;
		std::uint32_t* m_frame_count;
		types::get_native_handler* m_get_native_handler{};
		rage::scrNativeRegistrationTable* m_native_registration_table;
		types::fix_vectors m_fix_vector;
		rage::hash_list* m_vehicle_hash_pool;
		std::int64_t** m_script_globals{};
		types::ptr_to_handle m_ptr_to_handle;
		types::handle_to_ptr m_handle_to_ptr;
		bool* m_is_session_started;
		IDXGISwapChain** m_swapchain{};

	}; pointers* get_pointers();

#pragma pack(push, 1)
#	pragma warning(push)
#	pragma warning(disable : 4201) // nonstandard extension used: nameless struct/union
	union netAddress
	{
		uint32_t m_raw;
		struct
		{
			uint8_t m_field4;
			uint8_t m_field3;
			uint8_t m_field2;
			uint8_t m_field1;
		};
	};
#	pragma warning(pop)
	class netPlayerData
	{
	public:
		char pad_0000[8]; //0x0000
		int64_t m_rockstar_id_0; //0x0008
		char pad_0010[56]; //0x0010
		uint16_t N000005BF; //0x0048
		char pad_004A[2]; //0x004A
		netAddress m_online_ip; //0x0054
		int16_t m_online_port; //0x0058
		netAddress m_relay_ip; //0x004C
		int32_t m_relay_port; //0x0050
		char pad_005A[6]; //0x005A
		uint32_t m_host_token; //0x0060
		char pad_0064[12]; //0x0064
		int64_t m_rockstar_id; //0x0070
		char pad_0078[12]; //0x0078
		const char* m_name; //0x0084
	}; //Size: 0x0098
	class netPlayer
	{
	public:
		virtual ~netPlayer() = default;            // 0 (0x00)
		virtual void reset() = 0;                  // 1 (0x08)
		virtual bool is_valid() const = 0;         // 2 (0x10)
		virtual const char* get_name() const = 0;  // 3 (0x18)
		virtual void _0x20() = 0;                  // 4 (0x20)
		virtual bool is_host() = 0;                // 5 (0x28)
		virtual netPlayerData* get_net_data() = 0; // 6 (0x30)
		virtual void _0x38() = 0;                  // 7 (0x38)
	};
	class CPlayerInfo
	{
	public:
		char pad_0000[32]; //0x0000
		uint32_t m_internal_ip; //0x0020
		uint16_t m_internal_port; //0x0024
		char pad_0026[2]; //0x0026
		uint32_t m_relay_ip; //0x0028
		uint16_t m_relay_port; //0x002C
		char pad_002E[66]; //0x002E
		uint32_t N00000124; //0x0070
		uint32_t m_external_ip; //0x0074
		uint16_t m_external_port; //0x0078
		char pad_007A[22]; //0x007A
		uint64_t m_rockstar_id; //0x0090
		char pad_0098[12]; //0x0098
		char sName[20]; //0x00A4
		char pad_00B8[184]; //0x00B8
		float m_swim_speed; //0x0170
		char pad_0174[165]; //0x0174
		uint32_t m_frame_flags; //0x0218
		char pad_021D[1644]; //0x021D
		int32_t m_wanted_level; //0x0888
		int32_t m_wanted_level_2; //0x088C
		char pad_0890[1120]; //0x0890
		float m_run_speed; //0x0CF0
		float m_stamina; //0x0CF4
		float m_stamina_regen; //0x0CF8
	}; //Size=0x0C08
	class CNetGamePlayers : public netPlayer
	{
	public:
		char pad_0x0008[0x10]; //0x0008
		std::uint32_t msg_id; // 0x18
		char pad_0x001C[0x4]; //0x001C
		std::int8_t active_id; //0x0020 
		std::int8_t m_player_id; //0x0021 
		char pad_0x0022[0x6E]; //0x0022
		BYTE local_player_check;//0x0090
		char pad_0x00A1[0xF]; //0x0091
		CPlayerInfo* PlayerInfo; //0x00A0

		inline bool is_local_player()
		{
			return local_player_check & 1;
		}
	};
}