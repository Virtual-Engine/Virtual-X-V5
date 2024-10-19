#pragma once
#include "../common.hpp"

namespace rage {
	using joaat_t = uint32_t;
	inline constexpr char tolower(char c) {
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}
	inline constexpr char toUpper(char c) {
		return (c >= 'a' && c <= 'z') ? c + ('A' - 'a') : c;
	}
	inline uint32_t joaat(std::string_view str) {
		uint32_t hash = 0;
		for (auto c : str)
			hash += tolower(c), hash += (hash << 10), hash ^= (hash >> 6);
		hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);
		return hash;
	}


	
}
template <typename string_view_t = std::string_view>
inline consteval uint32_t constexprJoaat(string_view_t str, bool const forceLowerCase = true) {
	uint32_t hash{};
	while (*str != '\0') {
		hash += forceLowerCase ? rage::tolower(*str) : *str, ++str;
		hash += hash << 10, hash ^= hash >> 6;
	}
	hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);

	return hash;
}
template <size_t arraySize>
inline consteval uint32_t joaatFromCharArray(char const(&str)[arraySize]) {
	uint32_t hash{};
	for (int i{}; i != arraySize; ++i) {
		hash += rage::tolower(str[i]);
		hash += hash << 10, hash ^= hash >> 6;
	}
	hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);

	return hash;
}

inline consteval uint32_t operator ""_joaat(const char* str, size_t) { return constexprJoaat(str); }

template <std::size_t arrayS>
struct strData {
	constexpr strData(char const(&val)[arrayS]) { std::copy_n(val, arrayS, data); }
	char data[arrayS]{};
};
template <size_t finalLen>
struct concatData {
	constexpr concatData(std::string_view str1, std::string_view str2) {
		data[finalLen] = '\0';
		std::copy_n(str1.data(), str1.size(), data);
		std::copy_n(str2.data(), str2.size(), &data[str1.size()]);
	}
	char data[finalLen + 1]{};
};
template <strData str1, strData str2>
consteval auto constructConcatData() {
	return concatData<(sizeof(str1.data) - 1) + (sizeof(str2.data) - 1)>(str1.data, str2.data);
}
template <concatData previousConcat, strData str2>
consteval auto constructConcattedData() {
	return concatData<(sizeof(previousConcat.data) - 1) + (sizeof(str2.data) - 1)>(previousConcat.data, str2.data);
}
template <strData className>
consteval auto constructMangledRTTIString() {
	return constructConcattedData<constructConcatData<"?AV", className>(), "@@">();
}

consteval uint64_t generateMangledRTTIHashCode(std::string_view str) {
	auto const fnvOffsetBasis = 14695981039346656037ULL;
	auto const fnvPrime = 1099511628211ULL;
	auto basisCopy = fnvOffsetBasis;
	for (size_t i{}; i != str.size(); ++i) {
		basisCopy ^= static_cast<size_t>(str[i]);
		basisCopy *= fnvPrime;
	}
	basisCopy ^= basisCopy >> 32;
	return basisCopy;
}
static_assert(generateMangledRTTIHashCode("?AVCPlayerAppearanceDataNode@@") == 0x2AEE7E3A2D2E84FF);
template <strData className>
consteval auto generateRTTIHash() {
	return generateMangledRTTIHashCode(constructMangledRTTIString<className>().data);
}
static_assert(generateRTTIHash<"CPlayerAppearanceDataNode">() == 0x2AEE7E3A2D2E84FF);

#define generateRTTIHash(className) generateRTTIHash<STRINGIFY(className)>()

template <strData className>
consteval auto generateEncrtypredRAGE_RTTI() {
	return joaatFromCharArray(className.data);
}


#define RAGE_JOAAT_IMPL(str) (::rage::consteval_joaat(str))
#define RAGE_JOAAT(str) (std::integral_constant<rage::joaat_t, RAGE_JOAAT_IMPL(str)>::value)