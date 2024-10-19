#include "pattern_batch.hpp"

#include "../common.hpp"
#include "range.hpp"
#include "../utilities/log/log.hpp"

namespace VX {
	pattern_batch g_pattern_batch;
	void pattern_batch::add(std::string name, memory::pattern pattern, std::function<void(memory::handle)> callback) {
		m_entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
	}

	void pattern_batch::initialize(memory::range region) {
		auto timestamp = std::chrono::high_resolution_clock::now();
		bool all_found = true;
		for (auto& entry : m_entries) {
			if (auto result = region.scan(entry.m_pattern)) {
				if (entry.m_callback) {
					std::invoke(std::move(entry.m_callback), result);
				} else {
					all_found = false;
					get_log()->send("Memory", "Failed to find '{}'", entry.m_name);
				}
			} else {
				all_found = false;
				get_log()->send("Memory", "Failed to find '{}'",entry.m_name);
			}
		}
		get_log()->send("Memory", std::format("Took {}ms to scan {} patterns.", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timestamp).count(), m_entries.size()).c_str());

		m_entries.clear();
		if (!all_found) {
			throw std::runtime_error("Failed to find some patterns.");
		}
	}

	pattern_batch* get_batch() { return &g_pattern_batch; }
}
