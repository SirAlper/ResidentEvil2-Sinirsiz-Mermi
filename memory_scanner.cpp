#include <cstdint>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <iostream>

DWORD GetPIDByName(const std::string& processName)
{
    DWORD pid = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(hSnapshot, &pe32))
    {
        CloseHandle(hSnapshot);
        return 0;
    }

    do
    {
        if(processName == pe32.szExeFile)
        {
            pid = pe32.th32ProcessID;
            break; 
        }
    }while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return pid;
}


int main()
{
    std::string targetProcess = "re2.exe";

    DWORD pid = GetPIDByName(targetProcess);

    if(pid != 0)
    {
        std::cout << targetProcess << " bulundu! PID: " << pid << std::endl;
    }
    else
    {
        std::cout << targetProcess << " bulunamadi." << std::endl;
    }

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);

    if(hProcess == NULL)
    {
        std::cerr << "Surece baglanilamadi! Programi yonetici olarak baslattigindan emin ol." << std::endl;
    }
    else
    {
        std::cout << "Basariyla RE2'nin bellegine giris yapildi" << std::endl;
    }

    std::vector<uintptr_t> matches;
    MEMORY_BASIC_INFORMATION mbi;

    uintptr_t currentAdress = 0;

    while(VirtualQueryEx(hProcess, (LPCVOID)currentAdress, &mbi, sizeof(mbi)))
    {
        if(mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE)
        {
            std::vector<BYTE> buffer(mbi.RegionSize);
            SIZE_T bytesRead;
            if(ReadProcessMemory(hProcess, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead))
            {
                for(size_t i = 0; i <= bytesRead - sizeof(int); i += 4)
                {
                    int currentVal = *reinterpret_cast<int*>(&buffer[i]);

                    if(currentVal == 12)
                    {
                        uintptr_t foundAdress = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + i;
                        matches.push_back(foundAdress);
                    }
                }
            } 
        }

        currentAdress = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
    }

    std::cout << "Ilk arama bitti! Bulunan adres sayisi: " << matches.size() << std::endl;

    while(matches.size() > 3 )
    {

    
    std::cout << ">>> Oyuna donup 1 el ates edin." << std::endl;
    std::cout << ">>> Ates ettikten sonra konsola kalan merminizi yazip ENTER tusuna basin: " << std::endl;

    int newAmmo;
    std::cin >> newAmmo;

    std::cout << "ikinci filtreleme baslatiliyor..." << std::endl;

    std::vector<uintptr_t> filtiredMatches;

    for(uintptr_t addr : matches)
    {
        int currentVal = 0;
        SIZE_T bytesRead;

        if(ReadProcessMemory(hProcess, (LPCVOID)addr, &currentVal, sizeof(int), &bytesRead))
        {
            if(currentVal == newAmmo )
            {
                filtiredMatches.push_back(addr);
            }
        }
    }

    matches = filtiredMatches;

    std::cout << "Filtreleme bitti kalan adres sayisi: " << matches.size() << std::endl;
    }

    std::cout << "=== GERCEK MERMI ADRESLERI ===" << std::endl;
    
    for(uintptr_t addr : matches)
    {
        std::cout << "0x" << std::hex << std::uppercase << addr << std::dec << std::endl;
    }

    CloseHandle(hProcess);
    std::cout << "Handle guvenli bir sekilde kapatildi" << std::endl;

    return 0;

}
