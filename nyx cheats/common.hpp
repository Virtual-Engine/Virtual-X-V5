#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define CURL_STATICLIB

#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <minwindef.h>
#include <filesystem>
#include <mutex>
#include <functional>

#include <vector>

#include "pointers/pointers.hpp"

using namespace std::chrono_literals;

#define NAME "Virtual X"
#define VERSION "1.0.1 SP"

typedef const char* String;

struct Texture {
	String dict;
	String id;

	bool isEmpty() { return strcmp(dict, "") + strcmp(id, "") == 0; }
};

struct ModuleDetection {
	uint64_t m_base;
	uint64_t m_size;
	HMODULE m_module;
	char m_szName[100];

	ModuleDetection() {
	}

	ModuleDetection(uint64_t a, uint64_t b, const char* c, HMODULE hm) :
		m_base(a), m_size(b), m_module(hm) {
		strcpy(m_szName, c);
	}

	static friend bool operator==(const ModuleDetection& left, HMODULE module) {
		return left.m_module == module;
	}
};
inline char vaBuffer[0x1000];
inline char* va(String fmt, ...) {
	memset(vaBuffer, 0, 0x1000);
	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(vaBuffer, fmt, ap);
	va_end(ap);
	return vaBuffer;
}
typedef struct {
	float x, y, z, w;
} Vector4;

namespace VX {
	inline bool g_running = true;
	inline const char* m_menu_name = "Virtual X";
	inline int g_SelectedPlayer;
	namespace fs = std::filesystem;
}