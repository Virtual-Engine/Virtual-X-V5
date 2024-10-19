#include "manager.hpp"

namespace VX {
	fiber_manager g_fiber_manager;
	void fiber_manager::initialize() {
		static bool ensure_main_fiber = (ConvertThreadToFiber(nullptr), true);
		if (!ensure_main_fiber) {
			return;
		}
		for (auto const& script : g_fibers) {
			if (script->m_disabled)
				return;
			script->tick();
		}
	}
	void fiber_manager::add_fiber(fiber* script) {
		std::lock_guard Lock(m_Mutex);
		get_log()->send("Fibers", std::format("Create: {}", script->m_name).c_str());
		g_fibers.push_back(std::move(script));
	}
	void fiber_manager::cleanup() {
		for (auto& Fiber : g_fibers) {
			Fiber->m_disabled = true;
			DeleteFiber(Fiber->m_primary);
			DeleteFiber(Fiber->m_secondary);
		}
	}

	fiber_manager* get_fiber_manager() { return &g_fiber_manager; }
}