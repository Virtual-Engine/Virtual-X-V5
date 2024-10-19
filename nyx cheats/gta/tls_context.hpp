#pragma once
#include <intrin.h>
#include "../common.hpp"
#include "../gta/scr_threads.hpp"
#define DEFINE_RAGE_RTTI(className)    private:\
								virtual void* _0x00() = 0;\
								virtual void* _0x08() = 0;\
								virtual uint32_t _0x10() = 0;\
								virtual className* _0x18(void*) = 0;\
								virtual bool _0x20(void*) = 0;\
								virtual bool _0x28(void**) = 0;\
								virtual void destructor() = 0;\
								public:
#define offsetof(s,m) ((::size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
namespace rage
{
#pragma pack(push, 1)
	class tlsContext
	{
	public:
		char gap0[180];
		std::uint8_t m_unk_byte; // 0xB4
		char gapB5[3];
		sysMemAllocator* m_allocator; // 0xB8
		sysMemAllocator* m_allocator2; // 0xC0 - Same as 0xB8
		sysMemAllocator* m_allocator3; // 0xC8 - Same as 0xB8
		uint32_t m_console_smth; // 0xD0
		char gapD4[188];
		uint64_t m_unk; // 0x190
		char gap198[1728];
		rage::scrThread* m_script_thread; // 0x858
		bool m_is_script_thread_active; // 0x860

#if _WIN32
		static tlsContext* get()
		{
			constexpr std::uint32_t TlsIndex = 0x0;
			return *reinterpret_cast<tlsContext**>(__readgsqword(0x58) + TlsIndex);
		}
#endif
	};
	static_assert(sizeof(tlsContext) == 0x861);
#pragma pack(pop)
	template <typename T>
	class atRTTI {
	public:
		DEFINE_RAGE_RTTI(T)
	};
	class sysMemAllocator : public atRTTI<sysMemAllocator> {
	public:
		virtual ~sysMemAllocator() = 0;
		virtual void SetQuitOnFail(bool) = 0;
		virtual void* Allocate(size_t size, size_t align, int subAllocator) = 0;
		virtual void* TryAllocate(size_t size, size_t align, int subAllocator) = 0;
		virtual void Free(void* pointer) = 0;
		virtual void TryFree(void* pointer) = 0;
		virtual void Resize(void* pointer, size_t size) = 0;
		virtual sysMemAllocator* GetAllocator(int allocator) const = 0;
		virtual sysMemAllocator* GetAllocator(int allocator) = 0;
		virtual sysMemAllocator* GetPointerOwner(void* pointer) = 0;
		virtual size_t GetSize(void* pointer) const = 0;
		virtual size_t GetMemoryUsed(int memoryBucket) = 0;
		virtual size_t GetMemoryAvailable() = 0;
	public:
		static sysMemAllocator* UpdateAllocatorValue() {
			auto tls = rage::tlsContext::get();
			auto gtaTlsEntry = *reinterpret_cast<sysMemAllocator**>(uintptr_t(tls) + offsetof(rage::tlsContext, rage::tlsContext::get()->m_allocator));
			if (!gtaTlsEntry)
				tls->m_allocator = gtaTlsEntry;
			tls->m_allocator2 = gtaTlsEntry;
			return gtaTlsEntry;
		}
		static sysMemAllocator* get() {
			auto allocValue = rage::tlsContext::get()->m_allocator;
			if (!allocValue)
				allocValue = UpdateAllocatorValue();
			return allocValue;
		}
		static sysMemAllocator** getPointer() {
			auto tls = uintptr_t(*(uintptr_t*)__readgsqword(0x58));
			return reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, rage::tlsContext::get()->m_allocator2));
		}
		static sysMemAllocator* getEntry() {
			auto tls = uintptr_t(*(uintptr_t*)__readgsqword(0x58));
			return *reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, rage::tlsContext::get()->m_allocator2));
		}
		static sysMemAllocator** getEntryPointer() {
			auto tls = uintptr_t(*(uintptr_t*)__readgsqword(0x58));
			return reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, rage::tlsContext::get()->m_allocator2));
		}
	};
	class sysUseAllocator {
	public:
		void* operator new(size_t size) {
			return rage::sysMemAllocator::get()->Allocate(size, 16, NULL);
		}
		void* operator new[](size_t size) {
			return sysUseAllocator::operator new(size);
			}
			void operator delete(void* memory) {
				return rage::sysMemAllocator::get()->Free(memory);
			}
			void operator delete[](void* memory) {
				return sysUseAllocator::operator delete(memory);
				}
	};
}