#pragma once
#include <functional>
#include <vector>
#include "pattern.hpp"

namespace VX
{
	class pattern_batch
	{
	public:
		explicit pattern_batch() = default;
		~pattern_batch() noexcept = default;

		void add(std::string name, memory::pattern pattern, std::function<void(memory::handle)> callback);
		void initialize(memory::range region);
	
		struct entry
		{
			std::string m_name;
			memory::pattern m_pattern;
			std::function<void(memory::handle)> m_callback;

			explicit entry(std::string name, memory::pattern pattern, std::function<void(memory::handle)> callback) :
				m_name(std::move(name)),
				m_pattern(std::move(pattern)),
				m_callback(std::move(callback))
			{}
		};
	
	private:
		std::vector<entry> m_entries;
	};

	pattern_batch* get_batch();
}
