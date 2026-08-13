#include <handleapi.h>
#include <libloaderapi.h>
#include <memoryapi.h>
#include <minwinbase.h>
#include <processthreadsapi.h>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <winnt.h>


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
   const char* proccessName = "re2.exe";
    
   const char* dllPath = "OLUSTURDUGUNUZ DLL DOSYASININ YOLUNU BURAYA GIRIN";
   
   std::cout << "Uygulama Basladi" << std::endl; 

   DWORD procId = GetPIDByName(proccessName);
   if(procId == 0)
   {
       std::cerr << "Oyuna bulunmadi" << std::endl;
       return 1;
   }
   else
   {
       std::cout << "Oyun Bulundu" << std::endl;
  
   }
   
   HANDLE hProccess = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
  if(!hProccess)
  {
      std::cerr << "Oyuna erisim saglanamadi. Yonetici olarak calistirmayi deneyin." << std::endl;
      return 1;
  } 

   void* loc = VirtualAllocEx(hProccess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

   if(loc)
   {
       WriteProcessMemory(hProccess, loc, dllPath, strlen(dllPath) + 1, 0);

       HANDLE hThread = CreateRemoteThread(hProccess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

       if(hThread)
       {
           std::cout << "Basarili! DLL oyuna enjekte edildi." << std::endl;
           CloseHandle(hThread);
       }
       else
       {
           std::cerr << "Thread olusturulamadi!" << std::endl;
       }
   }
   CloseHandle(hProccess);
   system("pause");
   return 0;
}




