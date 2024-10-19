#pragma once
#include <map>
#include <functional>
#include "manager.hpp"
#include <stack>

namespace VX {
	class queue {
	public:
		void create_scripts() {
			for (auto i = 0; i < 8; i++) {
				get_fiber_manager()->g_fibers.push_back(new fiber(("fiber_" + std::to_string(i)).c_str(), [=] {
					while (true) {
						this->tick();
						fiber::current()->sleep();
					}
					}));
			}
		}
	public:
		void tick() {
			std::unique_lock lck(m_mutex);
			if (!m_jobs.empty()) {
				auto job = std::move(m_jobs.top());
				m_jobs.pop();
				lck.unlock();
				std::invoke(std::move(job));
			}
		}
		void add(std::function<void()> func) {
			std::lock_guard lck(m_mutex);
			m_jobs.push(std::move(func));
		}
	public:
		std::recursive_mutex m_mutex{};
		std::stack<std::function<void()>> m_jobs{};
	};
	inline queue g_queue{};
}