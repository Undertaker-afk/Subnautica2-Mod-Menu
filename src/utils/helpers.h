#pragma once

#include <Windows.h>
#include <string>

namespace Utils {
    namespace Memory {
        uintptr_t GetModuleBase(const wchar_t* moduleName);
        uintptr_t FindPattern(uintptr_t base, size_t size, const char* pattern, const char* mask);
        bool WriteMemory(uintptr_t address, const void* data, size_t size);
        bool ReadMemory(uintptr_t address, void* buffer, size_t size);
    }

    namespace Helpers {
        std::string WideStringToString(const wchar_t* wstr);
        std::wstring StringToWideString(const char* str);
        void Log(const char* format, ...);
    }
}
