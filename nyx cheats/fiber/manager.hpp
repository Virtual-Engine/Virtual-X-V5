#pragma once
#include "fiber.hpp"
#include "../utilities/log/log.hpp"

namespace VX {
	class fiber_manager
	{
	public:
		void initialize();
		void add_fiber(fiber* script);
		void cleanup();
	public:
		std::vector<fiber*> g_fibers;
	private:
		std::recursive_mutex m_Mutex;
	};

	fiber_manager* get_fiber_manager();
}
