#include "helpers.h"
#include <cstdarg>
#include <cstdio>

namespace Utils::Memory {
    uintptr_t GetModuleBase(const wchar_t* moduleName) {
        return reinterpret_cast<uintptr_t>(GetModuleHandleW(moduleName));
    }

    uintptr_t FindPattern(uintptr_t base, size_t size, const char* pattern, const char* mask) {
        size_t patternLen = strlen(mask);
        
        for (size_t i = 0; i < size - patternLen; i++) {
            bool found = true;
            for (size_t j = 0; j < patternLen; j++) {
                if (mask[j] != '?' && pattern[j] != reinterpret_cast<char*>(base + i)[j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return base + i;
            }
        }
        return 0;
    }

    bool WriteMemory(uintptr_t address, const void* data, size_t size) {
        DWORD oldProtect;
        if (VirtualProtect(reinterpret_cast<void*>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            memcpy(reinterpret_cast<void*>(address), data, size);
            VirtualProtect(reinterpret_cast<void*>(address), size, oldProtect, &oldProtect);
            return true;
        }
        return false;
    }

    bool ReadMemory(uintptr_t address, void* buffer, size_t size) {
        __try {
            memcpy(buffer, reinterpret_cast<void*>(address), size);
            return true;
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }
}

namespace Utils::Helpers {
    std::string WideStringToString(const wchar_t* wstr) {
        if (!wstr) return "";
        
        int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        if (size <= 0) return "";
        
        std::string result(size - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &result[0], size, nullptr, nullptr);
        return result;
    }

    std::wstring StringToWideString(const char* str) {
        if (!str) return L"";
        
        int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
        if (size <= 0) return L"";
        
        std::wstring result(size - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, str, -1, &result[0], size);
        return result;
    }

    void Log(const char* format, ...) {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        
        OutputDebugStringA(buffer);
        OutputDebugStringA("\n");
    }
}
