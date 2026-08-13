#include <cstddef>
#include <iterator>
#include <libloaderapi.h>
#include <memoryapi.h>
#include <minwinbase.h>
#include <minwindef.h>
#include <processthreadsapi.h>
#include <windows.h>
#include <vector>
#include <cstdint>
#include <winnt.h>

const BYTE pattern[] = {0x89, 0x41, 0x20, 0x48, 0x8B, 0x7C, 0x24, 0x40, 0x48, 0x8B, 0x5C, 0x24, 0x48, 0x48, 0x83, 0xC4, 0x20, 0x5E};

const char* mask = "xxxxxxxxxxxxxxxxxx"; 

uintptr_t ScanPattern(uintptr_t baseAddress, size_t size, const BYTE* sig, const char* mask)
{
    size_t patternLenght = strlen(mask);
    for(size_t i = 0; i < size - patternLenght; i++)
    {
        bool found = true;
        for(size_t j = 0; j < patternLenght; j++){
            if(mask[j] == 'x' && *(BYTE*)(baseAddress + i + j) != sig[j])
            {
                found = false;
                break;
            }

        }
        if (found)
        {
            return baseAddress + i;
        }
    }
    return 0;
}

void AppylInfiniteAmmo()
{
    HMODULE hModule = GetModuleHandle(NULL);
    if(!hModule) return;

    uintptr_t baseAddr = (uintptr_t)hModule;
    size_t scanSize = 0x10000000;
    
    uintptr_t ammoAddress = ScanPattern(baseAddr, scanSize, pattern, mask);

    if(ammoAddress){
        DWORD oldProtect;
        size_t patchSize = 3;

        VirtualProtect((void*)ammoAddress, patchSize, PAGE_EXECUTE_READWRITE, &oldProtect);

        memset((void*)ammoAddress, 0x90, patchSize);

        VirtualProtect((void*)ammoAddress, patchSize, oldProtect, &oldProtect);

    } 
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)AppylInfiniteAmmo, nullptr, 0 , nullptr);
        if(hThread != NULL)
        {
            CloseHandle(hThread);
        }
    }
    return TRUE;
}
