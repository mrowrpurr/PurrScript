#pragma once

#include <PurrScript/API.h>
#include <Windows.h>
#include <collections.h>

#include <filesystem>
#include <string_view>

namespace PurrScript::DLL {

    class DllLoader {
        using LOAD_FUNCTION_SIGNATURE = void (*)(PurrScriptAPI*);

        static constexpr auto LOAD_FUNCTION_NAME = "PurrScript_Load";

        collections_map<std::string, HMODULE> _loadedDlls;

    public:
        void LoadDLL(PurrScriptAPI* api, std::string_view path) {
            if (!std::filesystem::exists(path))
                throw std::runtime_error("File does not exist: " + std::string(path));

            auto dllPath = std::filesystem::path(path);
            auto dllName = dllPath.filename().string();

            if (_loadedDlls.contains(dllName)) return;

            auto dllHandle = LoadLibrary(path.data());
            if (dllHandle == nullptr) {
                auto errorCode = GetLastError();
                throw std::runtime_error("Failed to load DLL: " + std::to_string(errorCode));
            }

            auto loadFunction = reinterpret_cast<LOAD_FUNCTION_SIGNATURE>(
                GetProcAddress(dllHandle, LOAD_FUNCTION_NAME)
            );
            if (loadFunction == nullptr) {
                auto errorCode = GetLastError();
                throw std::runtime_error("Failed to load DLL: " + std::to_string(errorCode));
            }

            loadFunction(api);

            _loadedDlls.emplace(dllName, dllHandle);
        }

        void UnloadDLL(std::string_view path) {
            auto dllPath = std::filesystem::path(path);
            auto dllName = dllPath.filename().string();

            if (!_loadedDlls.contains(dllName)) return;

            auto dllHandle = _loadedDlls[dllName];
            FreeLibrary(dllHandle);

            _loadedDlls.erase(dllName);
        }
    };
}
