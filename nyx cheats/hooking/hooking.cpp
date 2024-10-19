#include "hooking.hpp"
#include "../pointers/pointers.hpp"
#include "hooks/script_virtual_machine.hpp"
#include "../rage/classes.hpp"
#include "../invoker/invoker.hpp"
#include "hooks/get_proc_adress.hpp"
namespace VX {

	hooking g_hooking;

	bool hooking::initialize() {
		MH_Initialize();

		m_script_virtual_machine.create("SVM", get_pointers()->m_script_virtual_machine, &hooks::script_virtual_machine);
		m_get_proc_adress.create("SHV", GetProcAddress, &hooks::get_proc_address);

		MH_EnableHook(MH_ALL_HOOKS);
		return true;
	}

	void hooking::cleanup() {
		m_script_virtual_machine.remove();
		m_get_proc_adress.remove();

		MH_DisableHook(MH_ALL_HOOKS);
		
		MH_Uninitialize();
	}
	hooking* get_hooking() { return &g_hooking; }
}