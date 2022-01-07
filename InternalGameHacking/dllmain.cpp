// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "stdafx.h"
#include "proc.h"
#include "mem.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    //CreateConsole
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT%", "w", stdout);

    std::cout << "OG for a free, stay sippin' fam\n";

    //get module base
    uintptr_t moduleBase = (uintptr_t)(GetModuleHandle(L"ex"));
    bool bHealth = false, bAmmo = false, bRecoil = false;

    //hack loop
    while (true)
    {
        //key input
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;

        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;
        }
        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bRecoil = !bRecoil;
            if (bRecoil)
            {
                //nop
                mem::Nop((BYTE*)moduleBase + 0x63786, 10);

            }
            else
            {
                //write back to original
                mem::Patch((BYTE*)moduleBase + 0x63786, (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
            }
        }
        //continue write/freeze
        uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10f4f4);

        if (localPlayerPtr)
        {
            if (bHealth)
            {
                *(int*)(*localPlayerPtr + 0xf8) = 9999;
            }
            if (bAmmo)
            {
                uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10f4f4, { 0x374, 0x14, 0x0 });
                int* ammo = (int*)ammoAddr;
                *ammo = 9999;
            }
        }
        Sleep(5);

    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));

    } 
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

