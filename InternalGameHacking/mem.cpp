#include "pch.h"
#include "mem.h"


void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	//variable to store the old permissions
	DWORD oldprotect;
	//get permission to read and write the memory
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	//write into the memory
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	//recover the old permissions
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);

}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	BYTE* nopArray = new BYTE[size];
	// 0x90 = nop
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;

}

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	//variable to store the old permissions
	DWORD oldprotect;
	//get permission to read and write the memory
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	//write into the memory
	memcpy(dst, src, size);
	//recover the old permissions
	VirtualProtect(dst, size, oldprotect, &oldprotect);

}

void mem::Nop(BYTE* dst, unsigned int size)
{
	//variable to store the old permissions
	DWORD oldprotect;
	//get permission to read and write the memory
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	//write "nop" into the memory
	memset(dst, 0x90, size);
	//recover the old permissions
	VirtualProtect(dst, size, oldprotect, &oldprotect);

}
uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}