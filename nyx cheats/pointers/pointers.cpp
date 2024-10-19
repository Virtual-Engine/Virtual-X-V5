#include "pointers.hpp"
#include "../hooking/hooking.hpp"
#include "../memory/all.hpp"

namespace VX {

	pointers g_pointers;

	void pointers::initialize() {

		get_batch()->add("LSS", "83 3D ? ? ? ? ? 75 17 8B 43 20 25", [this](memory::handle ptr) {
			this->m_loading_screen_state = ptr.add(2).rip().add(1).as<rage::eLoadingScreenState*>();
		});

		get_batch()->add("SVM", "E8 ? ? ? ? 48 85 FF 48 89 1D", [this](memory::handle ptr) {
			this->m_script_virtual_machine = ptr.add(1).rip().as<types::script_virtual_machine*>();
		});

		get_batch()->add("FC", "8B 15 ? ? ? ? 41 FF CF", [this](memory::handle ptr) {
			this->m_frame_count = ptr.add(2).rip().as<std::uint32_t*>();
		});

		get_batch()->add("NH", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", [this](memory::handle ptr) {
			this->m_native_registration_table = ptr.add(3).rip().as<rage::scrNativeRegistrationTable*>();
			this->m_get_native_handler = ptr.add(12).rip().as<types::get_native_handler*>();
		});

		get_batch()->add("FV", "83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA", [this](memory::handle ptr) {
			this->m_fix_vector = ptr.as<types::fix_vectors>();
		});

		get_batch()->add("VHP", "4C 03 05 ? ? ? ? EB 09 49 83 C2 04 EB 8B", [this](memory::handle ptr) {
			this->m_vehicle_hash_pool = ptr.add(3).rip().as<rage::hash_list*>();
		});

		get_batch()->add("SG", "48 8D 15 ? ? ? ? 4C 8B C0 E8 ? ? ? ? 48 85 FF 48 89 1D", [this](memory::handle ptr) {
			this->m_script_globals = ptr.add(3).rip().as<std::int64_t**>();
		});

		get_batch()->add("S", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", [this](memory::handle ptr) {
			this->m_swapchain = ptr.add(3).rip().as<IDXGISwapChain**>();
		});

		get_batch()->add("HTP", "83 F9 FF 74 31 4C 8B 0D", [this](memory::handle ptr) {
			this->m_handle_to_ptr = ptr.as<decltype(m_handle_to_ptr)>();
		});

		get_batch()->add("PTH", "48 8B F9 48 83 C1 10 33 DB", [this](memory::handle ptr) {
			this->m_ptr_to_handle = ptr.sub(0x15).as<decltype(m_ptr_to_handle)>();
		});

		get_batch()->add("ISS", "40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE", [this](memory::handle ptr) {
			this->m_is_session_started = ptr.add(3).rip().as<bool*>();
		});

		get_batch()->initialize(memory::module(nullptr));
	}

	pointers* get_pointers() { return &g_pointers; }
}