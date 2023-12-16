#pragma once

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

#include <PurrScript/API.h>
#include <collections.h>

#include <filesystem>
#include <string_view>

namespace PurrScript {

    class SharedLibraryLoader {
        using LOAD_FUNCTION_SIGNATURE = void (*)(PurrScriptAPI*);

#ifdef _WIN32
        using LibraryPtr = HMODULE;
#else
        using LibraryPtr = void*;
#endif

        static constexpr auto LOAD_FUNCTION_NAME = "PurrScript_Load";

        collections_map<std::string, HMODULE> _loadedDlls;

    public:
        void LoadSharedLibrary(PurrScriptAPI* api, std::string_view path) {
            if (!std::filesystem::exists(path))
                throw std::runtime_error("File does not exist: " + std::string(path));

            auto libraryPath = std::filesystem::path(path);
            auto libraryName = libraryPath.filename().string();

            if (_loadedDlls.contains(libraryName)) return;

            auto libraryHandle = LoadLibrary(path.data());
            if (libraryHandle == nullptr) {
                auto errorCode = GetLastError();
                throw std::runtime_error("Failed to load DLL: " + std::to_string(errorCode));
            }

            auto loadFunction = reinterpret_cast<LOAD_FUNCTION_SIGNATURE>(
                GetProcAddress(libraryHandle, LOAD_FUNCTION_NAME)
            );
            if (loadFunction == nullptr) {
                auto errorCode = GetLastError();
                throw std::runtime_error("Failed to load DLL: " + std::to_string(errorCode));
            }

            loadFunction(api);

            _loadedDlls.emplace(libraryName, libraryHandle);
        }

        void UnloadSharedLibrary(std::string_view path) {
            auto libraryPath = std::filesystem::path(path);
            auto libraryName = libraryPath.filename().string();

            if (!_loadedDlls.contains(libraryName)) return;

            auto libraryHandle = _loadedDlls[libraryName];
            FreeLibrary(libraryHandle);

            _loadedDlls.erase(libraryName);
        }
    };
}
